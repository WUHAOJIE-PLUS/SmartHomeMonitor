#include "ffmpegkits.h"
#include <QAudioFormat>
#include <QAudioSink>
#include <QDebug>
#include <QCoreApplication>
#include <QMutexLocker>

using std::mutex;

mutex audioMutex;

// 音频重采样缓冲区大小
#define AUDIO_BUFFER_SIZE 192000

FFmpegKits::FFmpegKits(QObject *parent)
    : QThread(parent), audioSink(nullptr), audioIO(nullptr)
{
    connect(this, &FFmpegKits::sigInitAudioDevice, this, &FFmpegKits::onInitAudioDevice);
    connect(this, &FFmpegKits::End, this, &FFmpegKits::cleanupResources);
}

FFmpegKits::~FFmpegKits()
{
    stopPlay(); // 确保线程停止
}

void FFmpegKits::startPlay(QString url)
{
    _url = url;
    this->start();
}

void FFmpegKits::stopPlay()
{
    if (isRunning()) {
        requestInterruption();
        quit();
        wait(); // 等待线程结束
    }
    // resetResources(); // 确保资源被释放
}

// 在 FFmpegKits 类中添加 resetResources 方法
void FFmpegKits::resetResources() {

    // 视频资源
    if (videoCodecCtx) {
        avcodec_free_context(&videoCodecCtx);
        videoCodecCtx = nullptr;
    }
    if (imgConvertCtx) {
        sws_freeContext(imgConvertCtx);
        imgConvertCtx = nullptr;
    }
    if (videoFrame) {
        av_frame_free(&videoFrame);
        videoFrame = nullptr;
    }
    if (videoFrameRGB) {
        av_frame_free(&videoFrameRGB);
        videoFrameRGB = nullptr;
    }
    if (videoBuffer) {
        av_free(videoBuffer);
        videoBuffer = nullptr;
    }

    // 音频资源
    if (swrCtx) {
        swr_free(&swrCtx);
        swrCtx = nullptr;
    }
    if (audioCodecCtx) {
        avcodec_free_context(&audioCodecCtx);
        audioCodecCtx = nullptr;
    }
    if (audioFrame) {
        av_frame_free(&audioFrame);
        audioFrame = nullptr;
    }
    if (audioBuffer) {
        av_free(audioBuffer);
        audioBuffer = nullptr;
    }

    // 音频设备
    if (audioSink) {
        audioSink->stop();
        delete audioSink;
        audioSink = nullptr;
        audioIO = nullptr;
    }
}

void FFmpegKits::onInitAudioDevice(int sampleRate, int channels)
{


    // 先停止音频
    if(audioSink) {
        audioSink->stop();
        // 使用互斥锁保护
        QMutexLocker locker(&audioMutex);
        delete audioSink;
        audioSink = nullptr;
    }

    // 确保在主线程中初始化音频设备
    QAudioFormat format;
    format.setSampleRate(sampleRate);
    format.setChannelCount(channels);
    format.setSampleFormat(QAudioFormat::Int16);

    if (audioSink) {
        audioSink->stop();
        delete audioSink;
        audioSink = nullptr;
    }

    audioSink = new QAudioSink(format);
    audioIO = audioSink->start();
    qDebug() << "Audio device initialized:" << sampleRate << "Hz," << channels << "channels";
}

void FFmpegKits::run()
{
    AVFormatContext *pFormatCtx = nullptr;
    AVDictionary *avdic = nullptr;
    av_dict_set(&avdic, "rtsp_transport", "tcp", 0);  // macOS推荐使用TCP[1](@ref)
    av_dict_set(&avdic, "max_delay", "100", 0);
    av_dict_set(&avdic, "stimeout", "3000000", 0);   // 设置超时3秒

    // 初始化FFmpeg
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    // 打开媒体流
    int ret = avformat_open_input(&pFormatCtx, _url.toUtf8().constData(), nullptr, &avdic);
    if (ret < 0) {
        qDebug() << "Could not open input:" << av_err2str(ret);
        emit sigError("无法连接媒体源");
        // emit End(pFormatCtx);
        cleanupResources(pFormatCtx); // 安全清理
        return; // 直接退出线程
    }

    // 查找流信息
    if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
        qDebug() << "Could not find stream information";
        emit sigError("无法获取流信息");

        // emit End(pFormatCtx);
        cleanupResources(pFormatCtx); // 安全清理
        return; // 直接退出线程
    }


    // 查找视频流和音频流
    int videoStreamIdx = -1;
    int audioStreamIdx = -1;

    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIdx == -1) {
            videoStreamIdx = i;
        } else if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audioStreamIdx == -1) {
            audioStreamIdx = i;
        }
    }

    // 初始化视频解码
    if (videoStreamIdx >= 0) {
        if (!initVideoDecoder(pFormatCtx, videoStreamIdx)) {
            emit sigError("视频解码器初始化失败");
            //清理资源
            // emit End(pFormatCtx);
            cleanupResources(pFormatCtx); // 安全清理
            return; // 直接退出线程
        }
    }

    // 初始化音频解码
    if (audioStreamIdx >= 0) {
        if (!initAudioDecoder(pFormatCtx, audioStreamIdx)) {
            qDebug() << "音频解码器初始化失败，继续播放视频";
        }
    }

    // 开始解码循环
    decodeLoop(pFormatCtx, videoStreamIdx, audioStreamIdx);


    // 在函数末尾添加
    cleanupResources(pFormatCtx);
    resetResources(); // 双重确保
    // return; // 直接退出线程
}

bool FFmpegKits::initVideoDecoder(AVFormatContext* formatCtx, int streamIdx)
{
    AVCodecParameters* codecPar = formatCtx->streams[streamIdx]->codecpar;
    const AVCodec* videoCodec = avcodec_find_decoder(codecPar->codec_id);
    if (!videoCodec) {
        qDebug() << "Unsupported video codec:" << codecPar->codec_id;
        return false;
    }

    videoCodecCtx = avcodec_alloc_context3(videoCodec);
    if (!videoCodecCtx) {
        qDebug() << "Failed to allocate video codec context";
        return false;
    }

    if (avcodec_parameters_to_context(videoCodecCtx, codecPar) < 0) {
        qDebug() << "Failed to copy video codec parameters";
        return false;
    }

    videoCodecCtx->thread_count = 4;  // 多线程解码
    if (avcodec_open2(videoCodecCtx, videoCodec, nullptr) < 0) {
        qDebug() << "Failed to open video codec";
        return false;
    }

    // 初始化SWS上下文用于YUV转RGB
    imgConvertCtx = sws_getContext(
        videoCodecCtx->width, videoCodecCtx->height, videoCodecCtx->pix_fmt,
        videoCodecCtx->width, videoCodecCtx->height, AV_PIX_FMT_RGB32,
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    // 分配帧内存
    videoFrame = av_frame_alloc();
    videoFrameRGB = av_frame_alloc();
    int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGB32, videoCodecCtx->width, videoCodecCtx->height, 1);
    videoBuffer = (uint8_t*)av_malloc(bufferSize);
    av_image_fill_arrays(videoFrameRGB->data, videoFrameRGB->linesize, videoBuffer,
                         AV_PIX_FMT_RGB32, videoCodecCtx->width, videoCodecCtx->height, 1);

    return true;
}

bool FFmpegKits::initAudioDecoder(AVFormatContext* formatCtx, int streamIdx)
{
    AVCodecParameters* codecPar = formatCtx->streams[streamIdx]->codecpar;
    const AVCodec* audioCodec = avcodec_find_decoder(codecPar->codec_id);
    if (!audioCodec) {
        qDebug() << "Unsupported audio codec:" << codecPar->codec_id;
        return false;
    }

    audioCodecCtx = avcodec_alloc_context3(audioCodec);
    if (!audioCodecCtx) {
        qDebug() << "Failed to allocate audio codec context";
        return false;
    }

    if (avcodec_parameters_to_context(audioCodecCtx, codecPar) < 0) {
        qDebug() << "Failed to copy audio codec parameters";
        return false;
    }

    if (avcodec_open2(audioCodecCtx, audioCodec, nullptr) < 0) {
        qDebug() << "Failed to open audio codec";
        return false;
    }

    // 初始化重采样上下文
    swrCtx = swr_alloc();
    av_opt_set_int(swrCtx, "in_channel_layout", audioCodecCtx->channel_layout, 0);
    av_opt_set_int(swrCtx, "out_channel_layout", audioCodecCtx->channel_layout, 0);
    av_opt_set_int(swrCtx, "in_sample_rate", audioCodecCtx->sample_rate, 0);
    av_opt_set_int(swrCtx, "out_sample_rate", audioCodecCtx->sample_rate, 0);
    av_opt_set_sample_fmt(swrCtx, "in_sample_fmt", audioCodecCtx->sample_fmt, 0);
    av_opt_set_sample_fmt(swrCtx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
    swr_init(swrCtx);

    // 初始化音频帧
    audioFrame = av_frame_alloc();
    audioBuffer = (uint8_t*)av_malloc(AUDIO_BUFFER_SIZE);

    // 通知主线程初始化音频设备
    emit sigInitAudioDevice(audioCodecCtx->sample_rate, audioCodecCtx->channels);

    return true;
}

void FFmpegKits::decodeLoop(AVFormatContext* formatCtx, int videoIdx, int audioIdx)
{
    AVPacket* packet = av_packet_alloc();
    int64_t lastVideoPts = AV_NOPTS_VALUE;
    int64_t lastAudioPts = AV_NOPTS_VALUE;

    while (!isInterruptionRequested()) {
        int ret = av_read_frame(formatCtx, packet);
        if (ret < 0) {
            if (ret == AVERROR_EOF) {
                qDebug() << "End of stream";
                break;
            }
            if (ret == AVERROR(EAGAIN)) continue;
            qDebug() << "Read frame error:" << av_err2str(ret);
            break;
        }

        // 视频包处理
        if (videoIdx >= 0 && packet->stream_index == videoIdx) {
            ret = avcodec_send_packet(videoCodecCtx, packet);
            if (ret < 0 && ret != AVERROR(EAGAIN)) {
                qDebug() << "Video send packet error:" << av_err2str(ret);
                av_packet_unref(packet);
                continue;
            }

            while (ret >= 0) {
                ret = avcodec_receive_frame(videoCodecCtx, videoFrame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
                if (ret < 0) {
                    qDebug() << "Video receive frame error:" << av_err2str(ret);
                    break;
                }

                // 转换YUV到RGB
                sws_scale(imgConvertCtx, videoFrame->data, videoFrame->linesize, 0,
                          videoCodecCtx->height, videoFrameRGB->data, videoFrameRGB->linesize);

                // 发送视频帧信号
                QImage image(videoBuffer, videoCodecCtx->width, videoCodecCtx->height, QImage::Format_RGB32);
                emit sigGetOneFrame(image.copy());

                // 更新时间戳
                if (videoFrame->pts != AV_NOPTS_VALUE) {
                    lastVideoPts = videoFrame->pts;
                }
            }
        }
        // 音频包处理
        else if (audioIdx >= 0 && packet->stream_index == audioIdx) {
            ret = avcodec_send_packet(audioCodecCtx, packet);
            if (ret < 0 && ret != AVERROR(EAGAIN)) {
                qDebug() << "Audio send packet error:" << av_err2str(ret);
                av_packet_unref(packet);
                continue;
            }

            while (ret >= 0) {
                ret = avcodec_receive_frame(audioCodecCtx, audioFrame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
                if (ret < 0) {
                    qDebug() << "Audio receive frame error:" << av_err2str(ret);
                    break;
                }

                // 重采样音频
                uint8_t* out = audioBuffer;
                int outSamples = swr_convert(swrCtx, &out, audioFrame->nb_samples,
                                             (const uint8_t**)audioFrame->data, audioFrame->nb_samples);
                if (outSamples > 0) {
                    int dataSize = outSamples * audioCodecCtx->channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

                    // 写入音频设备
                    if (audioIO) {
                        audioIO->write((const char*)audioBuffer, dataSize);
                    }

                    // 更新时间戳
                    if (audioFrame->pts != AV_NOPTS_VALUE) {
                        lastAudioPts = audioFrame->pts;
                    }
                }
            }
        }

        av_packet_unref(packet);
    }

    av_packet_free(&packet);
}

void FFmpegKits::cleanupResources(AVFormatContext* formatCtx)
{

    // 清理视频资源
    if (imgConvertCtx) sws_freeContext(imgConvertCtx);
    if (videoCodecCtx) {
        avcodec_close(videoCodecCtx);
        avcodec_free_context(&videoCodecCtx);
    }
    if (videoFrame) av_frame_free(&videoFrame);
    if (videoFrameRGB) av_frame_free(&videoFrameRGB);
    if (videoBuffer) av_free(videoBuffer);

    // 清理音频资源
    if (swrCtx) swr_free(&swrCtx);
    if (audioCodecCtx) {
        avcodec_close(audioCodecCtx);
        avcodec_free_context(&audioCodecCtx);
    }
    if (audioFrame) av_frame_free(&audioFrame);
    if (audioBuffer) av_free(audioBuffer);

    // 关闭音频设备
    if (audioSink) {
        audioSink->stop();
        delete audioSink;
        audioSink = nullptr;
        audioIO = nullptr;
    }

    // 关闭媒体源
    if (formatCtx) {
        avformat_close_input(&formatCtx);
        avformat_free_context(formatCtx);
    }

    // 重置指针
    imgConvertCtx = nullptr;
    videoCodecCtx = nullptr;
    videoFrame = nullptr;
    videoFrameRGB = nullptr;
    videoBuffer = nullptr;
    swrCtx = nullptr;
    audioCodecCtx = nullptr;
    audioFrame = nullptr;
    audioBuffer = nullptr;
    imgConvertCtx = nullptr;

    avformat_network_deinit();
}
