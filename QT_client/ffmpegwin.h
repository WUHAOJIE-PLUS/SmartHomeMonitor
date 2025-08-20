#ifndef FFMPEGWIN_H
#define FFMPEGWIN_H

#include <QWidget>
#include "ffwidget.h"

class FFMPEGWin : public QWidget
{
    Q_OBJECT
public:
    explicit FFMPEGWin(QWidget *parent = nullptr);
    void setStart_channel1(int);
    void setStart_channel2(int);
    void stop();
    void setVoice_mute();
private:
    QLabel* createChannelLabel(const QString &text);
    void IniUI();

private:
    VideoPlayer *left_up_video;
    VideoPlayer *right_up_video;
    VideoPlayer *left_down_video;
    VideoPlayer *right_down_video;

    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
};


#endif // FFMPEGWIN_H
