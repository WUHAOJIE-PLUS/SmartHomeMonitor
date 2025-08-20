#include "wininfo.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QPushButton>
#include <QHeaderView>

WinInfo::WinInfo(QWidget *parent)
    : QWidget{parent}
{
    IninUI();
}

void WinInfo::IninUI()
{

    setAttribute(Qt::WA_StyledBackground);//设置或清除指定的窗⼝部件属性,WA_StyledBackground,表⽰是否允许使⽤样式表来绘制窗⼝部件的背景
    setWindowFlags(Qt::FramelessWindowHint);

    this->setFixedSize(200,300);
    // this->setStyleSheet("background-color:white");


    QLabel *wininfo = new QLabel("窗口信息");
    wininfo->setAlignment(Qt::AlignCenter);  // 水平和垂直居中
    wininfo->setStyleSheet("background-color:rgb(54,54,54);color: white;");


    logTable = new QTableWidget;
    logTable->setColumnCount(2);
    logTable->setHorizontalHeaderLabels(QStringList() << "时间" << "消息");
    // 设置表格样式
    logTable->setStyleSheet(
        "QTableWidget {"
        "   background-color: #363636;"
        "   color: white;"
        "   border: none;"
        "   gridline-color: #505050;"
        "}"
        "QHeaderView::section {"
        "   background-color: #1E90FF;"
        "   color: white;"
        "   padding: 4px;"
        "   border: none;"
        "}"
        );

    // 设置表格属性
    logTable->verticalHeader()->setVisible(false); // 隐藏垂直表头
    logTable->setShowGrid(false); // 隐藏网格线
    logTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择
    logTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    // 设置表格无边框
    logTable->setFrameShape(QFrame::NoFrame);

    // // 设置列宽
    logTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // 时间列自适应
    logTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // 消息列拉伸
    logTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏滚动条

    // 创建控制按钮
    QPushButton *clearBtn = new QPushButton("清空日志", this);


    // 按钮样式
    clearBtn->setStyleSheet("background-color: #FF6347; color: white;");

    // 按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(clearBtn);


    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0); // 设置边距为0
    mainLayout->setSpacing(0);// 设置控件之间的间距为0
    mainLayout->addWidget(wininfo);
    mainLayout->addWidget(logTable);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    // 连接信号槽
    connect(clearBtn, &QPushButton::clicked, this, &WinInfo::clearLog);
}

void WinInfo::addLogEntry(const QString &message) {
    // 获取当前时间
    QString time = QTime::currentTime().toString("HH:mm:ss");

    // 在表格顶部插入新行（最新日志在最上面）
    int row = logTable->rowCount();
    logTable->insertRow(0);

    // 设置时间单元格
    QTableWidgetItem *timeItem = new QTableWidgetItem(time);
    timeItem->setTextAlignment(Qt::AlignCenter);
    timeItem->setFlags(timeItem->flags() ^ Qt::ItemIsEditable); // 禁止编辑
    logTable->setItem(0, 0, timeItem);

    // 设置消息单元格
    QTableWidgetItem *msgItem = new QTableWidgetItem(message);
    msgItem->setFlags(msgItem->flags() ^ Qt::ItemIsEditable); // 禁止编辑
    logTable->setItem(0, 1, msgItem);

    // 自动滚动到顶部（最新日志）
    logTable->scrollToTop();
}

void WinInfo::clearLog() {
    logTable->setRowCount(0); // 清空所有行
    addLogEntry("清空了所有操作日志");
}
