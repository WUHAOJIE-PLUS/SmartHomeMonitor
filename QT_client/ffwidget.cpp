#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QMessageBox>
#include "ffwidget.h"

VideoPlayer::VideoPlayer(QWidget *parent)
:QWidget(parent)
,ffmpeg(new FFmpegKits(this))
{
    setupUI();
    connectSignals();
}

VideoPlayer::~VideoPlayer()
{
    stopPlayback(); // 确保停止线程
    ffmpeg->stopPlay(); // 确保线程安全停止
}

void VideoPlayer::setupUI() {
    setWindowFlags(Qt::FramelessWindowHint);

    // 隐藏控制面板，只保留视频显示
    videoLabel = new QLabel(this);
    videoLabel->setAlignment(Qt::AlignCenter);
    videoLabel->setStyleSheet("background-color: #444444;");

    // 音量控制
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(80);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(videoLabel);
    mainLayout->addWidget(volumeSlider);
    mainLayout->setContentsMargins(0,0,0,0);
    volumeSlider->hide();
}

void VideoPlayer::connectSignals() {
    // FFmpeg信号
    connect(ffmpeg, &FFmpegKits::sigGetOneFrame, this,[this](const QImage &image) {
        videoLabel->setPixmap(QPixmap::fromImage(image).scaled(
            videoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation
            ));
    },Qt::QueuedConnection);

    connect(ffmpeg, &FFmpegKits::sigError, this, [this](const QString &msg) {
        QMessageBox::critical(this, "播放错误", msg);
    });

    // 音量控制
    connect(volumeSlider, &QSlider::valueChanged, this, [this](int value) {
        if (ffmpeg->getQAudioSink()) {
            ffmpeg->getQAudioSink()->setVolume(value / 100.0);
        }
    });
}

void VideoPlayer::startPlayback(int channel) {

    volumeSlider->show();

    QString url;
    if(channel == 1){
          url = "rtsp://admin:admin@192.168.5.222/live/chn=0";
    }else{
          url = "rtsp://admin:admin@192.168.5.222/live/chn=1";
    }

    if (url.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入有效的视频流地址");
        return;
    }

    ffmpeg->startPlay(url);
}



void VideoPlayer::stopPlayback() {
    ffmpeg->stopPlay();
    videoLabel->clear();
}

void VideoPlayer::setVoice(int value){

    if (ffmpeg->getQAudioSink()) {
        qDebug() << "test voice value: " << value;
        ffmpeg->getQAudioSink()->setVolume(value / 100.0);
    }
}

void VideoPlayer::resizeEvent(QResizeEvent *event)  {
    QWidget::resizeEvent(event);
    // 窗口大小变化时重绘视频帧
    if (!videoLabel->pixmap().isNull()) {
        QImage current = videoLabel->pixmap().toImage();
        videoLabel->setPixmap(QPixmap::fromImage(current).scaled(
            videoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation
            ));
    }
}

