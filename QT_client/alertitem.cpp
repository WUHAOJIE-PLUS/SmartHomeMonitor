#include "alertitem.h"
#include <QHBoxLayout>
#include <QLabel>

AlertItem:: AlertItem(const QString &time, const QString &event, const QString &status, const QPixmap &thumbnail, QWidget *parent)
    : QWidget(parent) {

    setAttribute(Qt::WA_StyledBackground);//设置或清除指定的窗⼝部件属性,WA_StyledBackground,表⽰是否允许使⽤样式表来绘制窗⼝部件的背景
    setWindowFlags(Qt::FramelessWindowHint);

    this->setFixedWidth(200);

    // 布局：左侧文字区域 + 右侧缩略图
    QHBoxLayout *layout = new QHBoxLayout(this);

    // 左侧文字区域（垂直布局）
    QVBoxLayout *textLayout = new QVBoxLayout;

    // 时间标签（蓝色）
    QLabel *timeLabel = new QLabel(time);
    timeLabel->setStyleSheet("color: #1E90FF; font-weight: bold;");

    // 事件描述（黄色）
    QLabel *eventLabel = new QLabel(event);
    eventLabel->setStyleSheet("color: #FFD700;");

    // 状态（灰色）
    QLabel *statusLabel = new QLabel(status);
    statusLabel->setStyleSheet("color: #A9A9A9;");

    textLayout->addWidget(timeLabel);
    textLayout->addWidget(eventLabel);
    textLayout->addWidget(statusLabel);

    // 右侧缩略图
    QLabel *thumbLabel = new QLabel;
    thumbLabel->setPixmap(thumbnail.scaled(60, 40, Qt::KeepAspectRatio));

    layout->addLayout(textLayout, 1); // 文字区域拉伸
    layout->addWidget(thumbLabel, 0, Qt::AlignRight); // 缩略图右对齐

    // 设置整体样式
    // setStyleSheet("background-color: #363636; border-bottom: 1px solid #505050;");
    setStyleSheet("background-color: #363636;");


    setLayout(layout);


}

