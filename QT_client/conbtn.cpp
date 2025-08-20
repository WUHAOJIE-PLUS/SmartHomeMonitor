#include "conbtn.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

ConBtn::ConBtn(QWidget *parent)
    : QWidget(parent)
    ,httpreq(new HttpReq(this))
    ,voice_icon(true)
{
    setlayout();
    applyStyles();
    setConnect();
}

void ConBtn::setlayout()
{


    this->setFixedWidth(200);

    label = new QLabel("云台控制",this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("border:2px;");

    zoom = new QLabel("变倍   ",this);
    focus = new QLabel("聚焦   ",this);
    light = new QLabel("光圈   ",this);


    // 设置按钮固定大小
    const int arrowBtnSize = 24;
    const int centerBtnSize = 24;

    btnUp = new QPushButton (this);
    btnUp->setObjectName("btnUp"); // 必须添加这行
    btnDown = new QPushButton (this);
    btnDown->setObjectName("btnDown");
    btnLeft = new QPushButton (this);
    btnLeft->setObjectName("btnLeft");
    btnRight = new QPushButton (this);
    btnRight->setObjectName("btnRight");
    btnUpLeft = new QPushButton (this);
    btnUpLeft->setObjectName("btnUpLeft");
    btnUpRight = new QPushButton (this);
    btnUpRight->setObjectName("btnUpRight");
    btnDownLeft = new QPushButton (this);
    btnDownLeft->setObjectName("btnDownLeft");
    btnDownRight = new QPushButton (this);
    btnDownRight->setObjectName("btnDownRight");
    btnCenter = new QPushButton (this);
    btnCenter->setObjectName("btnCenter");

    // voice_btn = new QPushButton (this);
    // voice_btn->setFixedSize(48, 48);
    // voice_btn->setObjectName("voice_btn");


    Zoom_add = new QPushButton(this);
    Zoom_add->setObjectName("Zoom_add"); // 必须添加这行
    Zoom_minus = new QPushButton(this);
    Zoom_minus->setObjectName("Zoom_minus");
    Focus_add = new QPushButton(this);
    Focus_add->setObjectName("Focus_add");
    Focus_minus = new QPushButton(this);
    Focus_minus->setObjectName("Focus_minus");
    ligth_add = new QPushButton(this);
    ligth_add->setObjectName("ligth_add");
    light_minus = new QPushButton(this);
    light_minus->setObjectName("light_minus");

    btnUp->setFixedSize(arrowBtnSize, arrowBtnSize);
    btnDown->setFixedSize(arrowBtnSize, arrowBtnSize);
    btnLeft->setFixedSize(arrowBtnSize, arrowBtnSize);
    btnRight->setFixedSize(arrowBtnSize, arrowBtnSize);
    btnUpLeft->setFixedSize(arrowBtnSize, arrowBtnSize);
    btnUpRight->setFixedSize(arrowBtnSize, arrowBtnSize);
    btnDownLeft->setFixedSize(arrowBtnSize, arrowBtnSize);
    btnDownRight->setFixedSize(arrowBtnSize, arrowBtnSize);
    btnCenter->setFixedSize(centerBtnSize, centerBtnSize);
    Zoom_add->setFixedSize(arrowBtnSize, arrowBtnSize);
    Zoom_minus->setFixedSize(arrowBtnSize, arrowBtnSize);
    Focus_add->setFixedSize(arrowBtnSize, arrowBtnSize);
    Focus_minus->setFixedSize(arrowBtnSize, arrowBtnSize);
    ligth_add->setFixedSize(arrowBtnSize, arrowBtnSize);
    light_minus->setFixedSize(arrowBtnSize, arrowBtnSize);

    QHBoxLayout *first_level = new QHBoxLayout;
    first_level->setSpacing(0); // 减小间距
    first_level->addWidget(btnUpLeft);
    first_level->addWidget(btnUp);
    first_level->addWidget(btnUpRight);

    QHBoxLayout *second_level = new QHBoxLayout;
    second_level->setSpacing(0);
    second_level->addWidget(btnLeft);
    second_level->addWidget(btnCenter);
    second_level->addWidget(btnRight);

    QHBoxLayout *third_level = new QHBoxLayout;
    third_level->setSpacing(0);
    third_level->addWidget(btnDownLeft);
    third_level->addWidget(btnDown);
    third_level->addWidget(btnDownRight);

    QHBoxLayout *Zoom_layout = new QHBoxLayout;
    Zoom_layout->addWidget(zoom);
    Zoom_layout->addWidget(Zoom_add);
    Zoom_layout->addSpacing(100);
    Zoom_layout->addWidget(Zoom_minus);
    Zoom_layout->addStretch();

    QHBoxLayout *Focus_layout = new QHBoxLayout;
    Focus_layout->addWidget(focus);
    Focus_layout->addWidget(Focus_add);
    Focus_layout->addSpacing(100);
    Focus_layout->addWidget(Focus_minus);
    Focus_layout->addStretch();


    QHBoxLayout *Light_layout = new QHBoxLayout;
    Light_layout->addWidget(light);
    Light_layout->addWidget(ligth_add);
    Light_layout->addSpacing(100);
    Light_layout->addWidget(light_minus);
    Light_layout->addStretch();


    QVBoxLayout *add_muins_layout = new QVBoxLayout;
    add_muins_layout->addLayout(Zoom_layout);
    add_muins_layout->addLayout(Focus_layout);
    add_muins_layout->addLayout(Light_layout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0); // 减小行间距
    layout->addWidget(label);
    layout->addLayout(first_level);
    layout->addLayout(second_level);
    layout->addLayout(third_level);
    layout->addSpacing(50);
    layout->addLayout(add_muins_layout);
    // layout->addSpacing(50);
    // layout->addWidget(voice_btn);
    layout->setContentsMargins(0, 0, 0, 0);//不要外层的边距

    this->setLayout(layout);

    label->setStyleSheet("border: 4px;background-color: black;color:white");
}

void ConBtn::applyStyles()
{
    QString style = R"(
        /* 全局样式 - 完全去除边距和边框 */
        QWidget {
            margin: 0px;
            padding: 0px;
            border: none;
        }

        /* 所有按钮通用样式 */
        QPushButton {
            background-repeat: no-repeat;
            background-position: center;
            border: none;
            background-color: transparent;
            margin: 0px;
            padding: 0px;
        }
        QPushButton:hover {
            background-color: #2980B9;
        }
        /* 上按钮 */
        #btnUp {
            background-image: url(:/image/111067_up_icon.png);
        }

        /* 下按钮 */
        #btnDown {
            background-image: url(:/image/111066_down_icon.png);
        }

        /* 左按钮 */
        #btnLeft {
            background-image: url(:/image/111065_left_icon.png);
        }

        /* 右按钮 */
        #btnRight {
            background-image: url(:/image/111064_right_icon.png);
        }

        /* 左上按钮 */
        #btnUpLeft {
            background-image: url(:/image/111062_up_arrow_left_icon.png);
        }

        /* 右上按钮 */
        #btnUpRight {
            background-image: url(:/image/111063_right_up_arrow_icon.png);
        }

        /* 左下按钮 */
        #btnDownLeft {
            background-image: url(:/image/111060_down_arrow_left_icon.png);
        }

        /* 右下按钮 */
        #btnDownRight {
            background-image: url(:/image/111061_down_right_arrow_icon.png);
        }

        /* 中心按钮 */
        #btnCenter {
            background-image: url(:/image/111057_reload_arrow_icon.png);
        }
        /* Zoom_add */
        #Zoom_add {
            background-image: url(:/image/3671645_add_solid_icon.png);
        }
        /* Zoom_minus */
        #Zoom_minus {
            background-image: url(:/image/3671809_minus_solid_icon.png);
        }
        /* Focus_add */
        #Focus_add {
            background-image: url(:/image/3671645_add_solid_icon.png);
        }
        /* Focus_minus */
        #Focus_minus {
            background-image: url(:/image/3671809_minus_solid_icon.png);
        }
        /* ligth_add */
        #ligth_add {
            background-image: url(:/image/3671645_add_solid_icon.png);
        }
        /* light_minus */
        #light_minus {
            background-image: url(:/image/3671809_minus_solid_icon.png);
        }
        /* voice_btn */
        #voice_btn {
            background-image: url(:/image/211939_high_volume_icon.png);
        }
    )";

    setStyleSheet(style);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground);// 禁止父窗口样式影响子控件样式
    setAttribute(Qt::WA_TranslucentBackground);//透明背景
}

void ConBtn::setConnect()
{
    // 连接信号槽
    connect(btnUp, &QPushButton::clicked, this, &ConBtn::onUpClicked);
    connect(btnDown, &QPushButton::clicked, this, &ConBtn::onDownClicked);
    connect(btnLeft, &QPushButton::clicked, this, &ConBtn::onLeftClicked);
    connect(btnRight, &QPushButton::clicked, this, &ConBtn::onRightClicked);
    connect(btnUpLeft, &QPushButton::clicked, this, &ConBtn::onUpLeftClicked);
    connect(btnUpRight, &QPushButton::clicked, this, &ConBtn::onUpRightClicked);
    connect(btnDownLeft, &QPushButton::clicked, this, &ConBtn::onDownLeftClicked);
    connect(btnDownRight, &QPushButton::clicked, this, &ConBtn::onDownRightClicked);
    connect(btnCenter, &QPushButton::clicked, this, &ConBtn::onCenterClicked);

    connect(Zoom_add, &QPushButton::clicked, this, &ConBtn::onZoomClicked_add);
    connect(Zoom_minus, &QPushButton::clicked, this, &ConBtn::onZoomClicked_muins);
    connect(Focus_add, &QPushButton::clicked, this, &ConBtn::onFocusClicked_add);
    connect(Focus_minus, &QPushButton::clicked, this, &ConBtn::onFocusClicked_muins);
    connect(voice_btn,&QPushButton::clicked,this,&ConBtn::change_voice_icon);
}

void ConBtn::onUpClicked()
{
    qDebug() << "向上移动";
    httpreq->movePTZ("u");
    emit sendInfo("云台向上移动");
    // 发送向上移动指令
}

void ConBtn::onDownClicked()
{
    qDebug() << "向下移动";
    httpreq->movePTZ("d");
    emit sendInfo("云台向下移动");
    // 发送向下移动指令
}

void ConBtn::onLeftClicked()
{
    qDebug() << "向左移动";
    httpreq->movePTZ("l");
    emit sendInfo("云台向左移动");
    // 发送向左移动指令
}

void ConBtn::onRightClicked()
{
    qDebug() << "向右移动";
    httpreq->movePTZ("r");
    emit sendInfo("云台向右移动");
    // 发送向右移动指令
}

void ConBtn::onUpLeftClicked()
{
    qDebug() << "向左上移动";
    httpreq->movePTZ("1");
    emit sendInfo("云台向左上移动");
    // 发送向左上移动指令
}

void ConBtn::onUpRightClicked()
{
    qDebug() << "向右上移动";
    httpreq->movePTZ("2");
    emit sendInfo("云台向右上移动");
    // 发送向右上移动指令
}

void ConBtn::onDownLeftClicked()
{
    qDebug() << "向左下移动";
    httpreq->movePTZ("3");
    emit sendInfo("云台向左下移动");
    // 发送向左下移动指令
}

void ConBtn::onDownRightClicked()
{
    qDebug() << "向右下移动";
    httpreq->movePTZ("4");
    emit sendInfo("云台向右下移动");
    // 发送向右下移动指令
}

void ConBtn::onCenterClicked()
{
    qDebug() << "中心复位";
    httpreq->movePTZ("s");//停止转动
    emit sendInfo("云台停止移动");
    // 发送复位指令
}

void ConBtn::onZoomClicked_add()
{
    qDebug() << "云台放大";
    httpreq->movePTZ("zoom in");
    emit sendInfo("云台放大变倍");
}

void ConBtn::onZoomClicked_muins()
{
    qDebug() << "云台缩小";
    httpreq->movePTZ("zoom out");
    emit sendInfo("云台缩小变倍");
}

void ConBtn::onFocusClicked_add()
{
    qDebug() << "聚焦增加";
    httpreq->movePTZ("focus in");
    emit sendInfo("云台增加聚焦");
}

void ConBtn::onFocusClicked_muins()
{
    qDebug() << "聚焦减小";
    httpreq->movePTZ("focus out");
    emit sendInfo("云台减小聚焦");
}

void ConBtn::onLightClicked_add()
{
    emit sendInfo("云台放大光圈");
}

void ConBtn::onLightClicked_muins()
{
    emit sendInfo("云台缩小光圈");
}

void ConBtn::change_voice_icon()
{

    if(voice_icon){
        //切换关闭图标
        voice_btn->setStyleSheet(" background-image: url(:/image/211942_volume_mute_icon.png);");
        voice_icon = false;
        emit voiceChange_mute();
    }else{
        //切换打开图标
        voice_btn->setStyleSheet(" background-image: url(:/image/211939_high_volume_icon.png);");
        voice_icon = true;
        emit voiceChange();
    }
}
