#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <framelesswidget.h>
#include <QLabel>
#include "ffmpegwin.h"
#include "alertwindow.h"
#include "conbtn.h"
#include "ctitlebar.h"
#include "wininfo.h"
#include "deviceList.h"

class CMainWindow : public FramelessWidget
{
    Q_OBJECT
public:
    CMainWindow(QWidget * parent= nullptr);
    void initUI();
    void Connect();
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private slots:
    void closeSlot();//与子对象CTitleBar::sigClose信号绑定
    void setVideoWin_Main(int);
    void setVideoWin_Sub(int);
    void Stop();
    void setWininfo(const QString &msg);
    void Changed_voice();
    void Changed_voice_mute();
private:
    AlertWindow *alterwin;
    ConBtn *conbtn;
    CTitleBar *ctitlebar;
    DeviceList *devicelis;
    FFMPEGWin *ffmwin;
    WinInfo *wininfo;
    QLabel *bottom_lab;

};

#endif // CMAINWINDOW_H
