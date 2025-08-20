#include "cmainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>

CMainWindow::CMainWindow(QWidget *parent)
:FramelessWidget(parent)
    ,alterwin(new AlertWindow(this))
    ,conbtn(new ConBtn(this))
    ,ctitlebar(new CTitleBar(this))
    ,devicelis(new DeviceList(this))
    ,ffmwin(new FFMPEGWin(this))
    ,wininfo(new WinInfo(this))
{
    initUI();
    Connect();
}


void CMainWindow::closeSlot()
{
    static bool inProgress = false;
    if (inProgress) return;
    inProgress = true;

    // macOS 专用样式
    QString styleSheet = R"(
    QMessageBox {
        background-color: rgba(245, 245, 247, 0.95);
        border: 1px solid rgba(0, 0, 0, 0.1);
        border-radius: 12px;
        padding: 25px;
        font-family: -apple-system, BlinkMacSystemFont, "Helvetica Neue", sans-serif;
        box-shadow: 0 10px 30px rgba(0, 0, 0, 0.1);
    }

    QMessageBox QLabel#qt_msgbox_label {
        color: #333;
        font-size: 16px;
        font-weight: 500;
        margin-bottom: 25px;
    }

    QMessageBox QPushButton {
        background-color: #007aff;
        color: white;
        border-radius: 6px;
        padding: 8px 24px;
        font-size: 14px;
        min-width: 80px;
    }

    QMessageBox QPushButton:hover {
        background-color: #0066cc;
    }

    QMessageBox QPushButton:pressed {
        background-color: #0055aa;
    }

    QMessageBox QPushButton:focus {
        outline: none;
        box-shadow: 0 0 0 2px rgba(0, 122, 255, 0.3);
    }
)";

    // 创建消息框
    QMessageBox msgBox;
    msgBox.setWindowTitle("提示");
    msgBox.setText("你确定要退出吗？");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);


    // 添加淡入动画
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(&msgBox);
    msgBox.setGraphicsEffect(opacityEffect);

    QPropertyAnimation *anim = new QPropertyAnimation(opacityEffect, "opacity");
    anim->setDuration(300);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutQuad);
    anim->start();

    // 设置自定义警告图标
    QPixmap warningIcon(64, 64);
    warningIcon.fill(Qt::transparent);

    QPainter painter(&warningIcon);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(255, 204, 0)); // 黄色警告色
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, 64, 64);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 48, QFont::Bold));
    painter.drawText(warningIcon.rect(), Qt::AlignCenter, "!");

    msgBox.setIconPixmap(warningIcon);

    // 应用样式表
    msgBox.setStyleSheet(styleSheet);

    // 显示消息框
    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes) {
        Stop();//视频停止
        close();
    }

    inProgress = false;
}


void CMainWindow::initUI()
{
    setMinimumSize(1200, 800);

    bottom_lab = new QLabel("隔壁老王V2.0",this);
    bottom_lab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    bottom_lab->setStyleSheet("QLabel { background-color:rgb(54,54,54);color: rgba(255, 255, 255, 0.5); }");

    QVBoxLayout *left_layout = new QVBoxLayout;
    left_layout->setSpacing(0);
    left_layout->addWidget(wininfo);
    left_layout->addWidget(alterwin);
    left_layout->setContentsMargins(0,0,0,0);

    QVBoxLayout *rigth_layout = new QVBoxLayout;
    rigth_layout->setSpacing(0);
    rigth_layout->addWidget(devicelis);
    rigth_layout->addWidget(conbtn);
    rigth_layout->addStretch();
    rigth_layout->setContentsMargins(0,0,0,0);

    QHBoxLayout *mid_layout = new QHBoxLayout;
    mid_layout->setSpacing(0);
    mid_layout->addLayout(left_layout,1);
    mid_layout->addWidget(ffmwin,2);
    mid_layout->addLayout(rigth_layout,1);
    mid_layout->setContentsMargins(0,0,0,0);


    QVBoxLayout *mainlayout =  new QVBoxLayout;
    mainlayout->setSpacing(0);
    mainlayout->addWidget(ctitlebar);
    mainlayout->addLayout(mid_layout);
    mainlayout->addWidget(bottom_lab);
    mainlayout->setContentsMargins(0,0,0,0);

    this->setLayout(mainlayout);
}


void CMainWindow::Connect()
{
    connect(ctitlebar, &CTitleBar::sigClose, this, &CMainWindow::closeSlot);
    connect(devicelis,&DeviceList::cam_main,this,&CMainWindow::setVideoWin_Main);
    connect(devicelis,&DeviceList::cam_sub,this,&CMainWindow::setVideoWin_Sub);

    connect(devicelis,&DeviceList::sendInfo,this,&CMainWindow::setWininfo);
    connect(ctitlebar,&CTitleBar::sendInfo,this,&CMainWindow::setWininfo);
    connect(conbtn,&ConBtn::sendInfo,this,&CMainWindow::setWininfo);

    connect(conbtn,&ConBtn::voiceChange,this,&CMainWindow::Changed_voice);
    connect(conbtn,&ConBtn::voiceChange_mute,this,&CMainWindow::Changed_voice_mute);
}

void CMainWindow::setVideoWin_Main(int id)
{
    qDebug() << "main_id: " << id;
    ffmwin->setStart_channel1(id);
}


void CMainWindow::setVideoWin_Sub(int id)
{
    qDebug() << "sub_id: " << id;
    ffmwin->setStart_channel2(id);
}

void CMainWindow::Stop()
{
    ffmwin->stop();
}
void CMainWindow::setWininfo(const QString &msg ){
    wininfo->addLogEntry(msg);
}

void CMainWindow::Changed_voice()
{

}

void CMainWindow::Changed_voice_mute()
{
    ffmwin->setVoice_mute();
}


void CMainWindow::mouseDoubleClickEvent(QMouseEvent *event){
    if(this->isFullScreen()) {//是否全屏
        ctitlebar->show();
        this->showNormal();//退出全屏
        wininfo->addLogEntry("窗口退出全屏");
    }else{

        this->showFullScreen();//全屏
        ctitlebar->hide();
        wininfo->addLogEntry("窗口全屏");
    }
}

void CMainWindow::keyPressEvent(QKeyEvent *event) {
    qDebug() << "keyPressEvent";
    if(event->key() == Qt::Key_Escape) {
        ctitlebar->show();
        this->showNormal();//退出全屏
        wininfo->addLogEntry("窗口退出全屏");
    }
}



