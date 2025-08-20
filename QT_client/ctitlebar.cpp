#include "ctitlebar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTimer>
#include <QTime>


CTitleBar::CTitleBar(QWidget *parent)
    : QWidget{parent}
{
    initUI();
}

void CTitleBar::initUI()
{


//禁止父窗口影响子窗口样式,必须加上，否则样式不会起作用
    setAttribute(Qt::WA_StyledBackground);//设置或清除指定的窗⼝部件属性,WA_StyledBackground,表⽰是否允许使⽤样式表来绘制窗⼝部件的背景
    setWindowFlags(Qt::FramelessWindowHint);

    this->setFixedHeight(50);
    this->setStyleSheet("background-color:white");


    _plogoLabel = new QLabel(this);
    _plogoLabel->setFixedSize(32, 32);
    _plogoLabel->setStyleSheet("background-image:url(:/image/10219198_cctv_camera_security_secure_protection_icon _big.png);");

    _ptitleTextLabel = new QLabel(this);
    _ptitleTextLabel->setText("视频监控管理平台试用版");
    _ptitleTextLabel->setFixedWidth(150);


    _ptoolButton = new QPushButton(this);
    _ptoolButton->setFixedSize(48, 48);
    // 使用样式表设置背景图片和文字布局
    _ptoolButton->setStyleSheet(
        "QPushButton {"
        "   background-image: url(:/image/461381_settings_configuration_options_tool_tools_icon.png);"
        "   background-repeat: no-repeat;"
        "   border: none;"
        "   background-position: center top; /* 图片位于顶部居中 */"
        "   padding-top: 24px; /* 为文字预留顶部空间 */"
        "   text-align: bottom center; /* 文字底部居中 */"
        "   color: black; /* 文字颜色 */"
        "}"
        "QPushButton:hover {background-color: #2980B9;}"
        );
    _ptoolButton->setText("系统设置"); // 设置按钮文字

    _pfileButton = new QPushButton(this);
    _pfileButton->setFixedSize(48, 48);
    // 使用样式表设置背景图片和文字布局
    _pfileButton->setStyleSheet(
        "QPushButton {"
        "   background-image: url(:/image/9004781_file_document_paper_page_icon.png);"
        "   background-repeat: no-repeat;"
        "   border: none;"
        "   background-position: center top; /* 图片位于顶部居中 */"
        "   padding-top: 24px; /* 为文字预留顶部空间 */"
        "   text-align: bottom center; /* 文字底部居中 */"
        "   color: black; /* 文字颜色 */"
        "}"
        "QPushButton:hover {background-color: #2980B9;}"
        );
    _pfileButton->setText("日志查询"); // 设置按钮文字

    _pmapButton = new QPushButton(this);
    _pmapButton->setFixedSize(48, 48);
    // 使用样式表设置背景图片和文字布局
    _pmapButton->setStyleSheet(
        "QPushButton {"
        "   background-image: url(:/image/8665620_map_location_dot_icon.png);"
        "   background-repeat: no-repeat;"
        "   border: none;"
        "   background-position: center top; /* 图片位于顶部居中 */"
        "   padding-top: 24px; /* 为文字预留顶部空间 */"
        "   text-align: bottom center; /* 文字底部居中 */"
        "   color: black; /* 文字颜色 */"
        "}"
        "QPushButton:hover {background-color: #2980B9;}"
        );
    _pmapButton->setText("电子地图"); // 设置按钮文字

    _pveidoButton = new QPushButton(this);
    _pveidoButton->setFixedSize(48, 48);
    // 使用样式表设置背景图片和文字布局
    _pveidoButton->setStyleSheet(
        "QPushButton {"
        "   background-image: url(:/image/9055454_bxs_movie_play_icon.png);"
        "   background-repeat: no-repeat;"
        "   border: none;"
        "   background-position: center top; /* 图片位于顶部居中 */"
        "   padding-top: 24px; /* 为文字预留顶部空间 */"
        "   text-align: bottom center; /* 文字底部居中 */"
        "   color: black; /* 文字颜色 */"
        "}"
        "QPushButton:hover {background-color: #2980B9;}"
        );
    _pveidoButton->setText("视频回放"); // 设置按钮文字


    _pmonitorButton = new QPushButton(this);
    _pmonitorButton->setFixedSize(48, 48);
    // 使用样式表设置背景图片和文字布局
    _pmonitorButton->setStyleSheet(
        "QPushButton {"
        "   background-image: url(:/image/10219198_cctv_camera_security_secure_protection_icon.png);"
        "   background-repeat: no-repeat;"
        "   border: none;"
        "   background-position: center top; /* 图片位于顶部居中 */"
        "   padding-top: 24px; /* 为文字预留顶部空间 */"
        "   text-align: bottom center; /* 文字底部居中 */"
        "   color: black; /* 文字颜色 */"
        "}"
        "QPushButton:hover {background-color: #2980B9;}"
        );
    _pmonitorButton->setText("视频监控"); // 设置按钮文字



    // 时间显示
    timeLabel = new QLabel(QTime::currentTime().toString("  hh:mm:ss"));
    timeLabel->setObjectName("timeLabel");


    _pminButton = new QPushButton(this);
    _pminButton->setFixedSize(16, 16);
    _pminButton->setStyleSheet(
        "QPushButton{background-image:url(:/image/1303882_delete_minimize_minus_remove_sign_icon.png);border:none}"
        "QPushButton:hover {background-color: #2980B9;}"
        );

    _pmaxButton = new QPushButton(this);
    _pmaxButton->setFixedSize(16, 16);
    _pmaxButton->setStyleSheet(
    "QPushButton{background-image:url(:/image/8673614_ic_fluent_maximize_filled_icon.png);border:none}"
    "QPushButton:hover {background-color: #2980B9;}");

    _pcloseButton = new QPushButton(this);
    _pcloseButton->setFixedSize(16, 16);
    _pcloseButton->setStyleSheet(
        "QPushButton{background-image:url(:/image/1303884_cancel_close_cross_delete_exit_icon.png);border:none}"
        "QPushButton:hover {background-color: #2980B9;}");


    QHBoxLayout *layoout_time = new QHBoxLayout;
    layoout_time->addWidget(_pminButton);
    layoout_time->addWidget(_pmaxButton);
    layoout_time->addWidget(_pcloseButton);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(layoout_time);
    vlayout->addWidget(timeLabel);


    QHBoxLayout * phLayout = new QHBoxLayout(this);
    phLayout->addWidget(_plogoLabel);
    phLayout->addWidget(_ptitleTextLabel);

    phLayout->addStretch();//添加一个弹簧。它直接在布局中添加⼀个可伸缩的空⽩区域

    phLayout->addWidget(_pmonitorButton);
    phLayout->addWidget(_pveidoButton);
    phLayout->addWidget(_pmapButton);
    phLayout->addWidget(_pfileButton);
    phLayout->addWidget(_ptoolButton);
    phLayout->addLayout(vlayout);
    phLayout->setContentsMargins(5,5,5,5);


    connect(_pminButton, &QPushButton::clicked, this, &CTitleBar::onClickedSlot);
    connect(_pmaxButton, &QPushButton::clicked, this, &CTitleBar::onClickedSlot);
    connect(_pcloseButton, &QPushButton::clicked, this, &CTitleBar::onClickedSlot);

    setStyleSheet(R"(
        QPushButton {
            background-color: #3498DB;
            color: white;
            border-radius: 4px;
            padding: 8px 16px;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: #2980B9;
        }
    )");



    // 更新时间显示
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=] {
        timeLabel->setText(QTime::currentTime().toString("  hh:mm:ss"));
    });
    timer->start(1000); // 每秒更新

}


void CTitleBar::onClickedSlot()
{
    QPushButton * pbtn = qobject_cast<QPushButton*>(sender());//获取是哪个按钮点击的
    QWidget * pwindow = this->window();
    if(pbtn == _pminButton) {//_pminButton点击
        pwindow->showMinimized();//设置为最小化
        emit sendInfo("屏幕最小化");
    } else if(pbtn == _pmaxButton) {//_pmaxButton点击
        if(pwindow->isMaximized()) {//判断此时状态是否为最大化
            pwindow->showNormal();//恢复正常
            emit sendInfo("屏幕恢复正常");
        } else  {
            pwindow->showMaximized();//设置为最大化
            emit sendInfo("屏幕最大化");
        }
    } else if(pbtn == _pcloseButton) {//_pcloseButton点击
        emit sigClose();//发射信号，通知父窗口关闭//如果在这里关闭的话，关闭的是标题栏，而不是整个app
        emit sendInfo("点击了退出");
    }
}
