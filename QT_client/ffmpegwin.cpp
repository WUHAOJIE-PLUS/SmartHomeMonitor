#include "ffmpegwin.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QLabel>

FFMPEGWin::FFMPEGWin(QWidget *parent)
    : QWidget(parent)
    , left_up_video(new VideoPlayer(this))
    , right_up_video(new VideoPlayer(this))
    , left_down_video(new VideoPlayer(this))
    , right_down_video(new VideoPlayer(this))
{
    IniUI();
}


void FFMPEGWin::IniUI()
{
    // 设置窗口最小尺寸
    setMinimumSize(800, 600);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground);

    // 设置深灰色背景 - 匹配第一张图片
    setStyleSheet("background-color: #333333;");

    // 创建主布局
    QGridLayout *main_layout = new QGridLayout(this);
    main_layout->setSpacing(0); // 无间距
    main_layout->setContentsMargins(0, 0, 0, 0); // 无边距

    // 添加通道标签 - 匹配第一张图片的文字样式
    label1 = createChannelLabel("通道01");
    label2 = createChannelLabel("通道02");
    label3 = createChannelLabel("通道03");
    label4 = createChannelLabel("通道04");

    // 添加视频播放器到网格布局
    main_layout->addWidget(left_up_video, 0, 0);
    main_layout->addWidget(label1, 0, 0, Qt::AlignCenter); // 添加标签到左上角
    main_layout->addWidget(right_up_video, 0, 1);
    main_layout->addWidget(label2, 0, 1, Qt::AlignCenter); // 添加标签到右上角
    main_layout->addWidget(left_down_video, 1, 0);
    main_layout->addWidget(label3, 1, 0, Qt::AlignCenter); // 添加标签到左下角
    main_layout->addWidget(right_down_video, 1, 1);
    main_layout->addWidget(label4, 1, 1, Qt::AlignCenter); // 添加标签到右下角

    // 设置布局比例 - 确保均匀分布
    main_layout->setColumnStretch(0, 1);
    main_layout->setColumnStretch(1, 1);
    main_layout->setRowStretch(0, 1);
    main_layout->setRowStretch(1, 1);

    main_layout->setContentsMargins(2,2,2,2);
     main_layout->setSpacing(0);

    // 设置视频播放器样式 - 匹配第一张图片的深灰色矩形
    QString videoStyle = "VideoPlayer {"
                         "   background-color: #444444;" // 深灰色背景
                         "   border: 2px solid #0078D7;" // 细边框
                         "}";
    left_up_video->setStyleSheet(videoStyle);
    right_up_video->setStyleSheet(videoStyle);
    left_down_video->setStyleSheet(videoStyle);
    right_down_video->setStyleSheet(videoStyle);

    // 设置主布局
    this->setLayout(main_layout);
}


void FFMPEGWin::setStart_channel1(int id)
{
    switch(id){
    case 1:
        left_up_video->startPlayback(1);
        label1->hide();
        break;
    case 2:
        right_up_video->startPlayback(1);
        label2->hide();
        break;
    case 3:
        left_down_video->startPlayback(1);
        label3->hide();
        break;
    case 4:
        right_down_video->startPlayback(1);
        label4->hide();
        break;
    }
}

void FFMPEGWin::setStart_channel2(int id)
{
    switch(id){
    case 1:
        left_up_video->startPlayback(2);
        label1->hide();
        break;
    case 2:
        right_up_video->startPlayback(2);
        label2->hide();
        break;
    case 3:
        left_down_video->startPlayback(2);
        label3->hide();
        break;
    case 4:
        right_down_video->startPlayback(2);
        label4->hide();
        break;
    }
}

void FFMPEGWin::stop()
{
    left_up_video->stopPlayback();
    right_up_video->stopPlayback();
    left_down_video->stopPlayback();
    right_down_video->stopPlayback();
}

void FFMPEGWin::setVoice_mute()
{
    left_up_video->setVoice(0);
    right_up_video->setVoice(0);
    left_down_video->setVoice(0);
    left_up_video->setVoice(0);
}

QLabel* FFMPEGWin::createChannelLabel(const QString &text)
{
    QLabel *label = new QLabel(text, this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "QLabel {"
        "   color: white;" // 白色文字
        "   font-family: 'Microsoft YaHei';"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   background: transparent;" // 透明背景
        "}"
        );
    return label;
}
