#include <QThread>
#include <QImage>
#include <QAudioSink>


extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/avutil.h>
#include <libavutil/parseutils.h>
#include <libavutil/time.h>
#include <libavutil/imgutils.h>  // 解决av_image_get_buffer_size等
#include <libavutil/opt.h>       // 解决av_opt_set_int等
}

class FFmpegKits : public QThread
{
    Q_OBJECT
public:
    explicit FFmpegKits(QObject *parent = nullptr);
    ~FFmpegKits();

    void startPlay(QString url);
    void stopPlay();
    QAudioSink* getQAudioSink(){ return audioSink;}

signals:
    void sigGetOneFrame(QImage image);
    void sigError(QString message);
    void sigInitAudioDevice(int sampleRate, int channels); // 内部信号

    void End(AVFormatContext *);
private slots:
    void onInitAudioDevice(int sampleRate, int channels);

protected:
    void run() override;


private:
    bool initVideoDecoder(AVFormatContext* formatCtx, int streamIdx);
    bool initAudioDecoder(AVFormatContext* formatCtx, int streamIdx);
    void decodeLoop(AVFormatContext* formatCtx, int videoIdx, int audioIdx);
    void cleanupResources(AVFormatContext* formatCtx);
    void resetResources();

private:
    // 视频相关
    AVCodecContext* videoCodecCtx = nullptr;
    SwsContext* imgConvertCtx = nullptr;
    AVFrame* videoFrame = nullptr;
    AVFrame* videoFrameRGB = nullptr;
    uint8_t* videoBuffer = nullptr;

    // 音频相关
    AVCodecContext* audioCodecCtx = nullptr;
    SwrContext* swrCtx = nullptr;
    AVFrame* audioFrame = nullptr;
    uint8_t* audioBuffer = nullptr;
    QAudioSink* audioSink = nullptr;
    QIODevice* audioIO = nullptr;


    QString _url;

};
