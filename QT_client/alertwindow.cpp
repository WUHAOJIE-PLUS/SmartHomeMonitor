#include "alertwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QPushButton>


AlertWindow::AlertWindow(QWidget *parent) : QWidget(parent) {

    setAttribute(Qt::WA_StyledBackground);//设置或清除指定的窗⼝部件属性,WA_StyledBackground,表⽰是否允许使⽤样式表来绘制窗⼝部件的背景
    setWindowFlags(Qt::FramelessWindowHint);

    this->setFixedWidth(200);

    // 标题
    QLabel *title = new QLabel("图文警情");
    title->setStyleSheet("background-color: #1E90FF; color: white; padding: 8px;");
    title->setAlignment(Qt::AlignCenter);

    // 列表控件
    list = new QListWidget;
    list->setStyleSheet("background-color: #2D2D2D; border: none;");
    list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏滚动条
    list->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑

    // 创建控制按钮
    QPushButton *clearBtn = new QPushButton("清空警情", this);
    // 按钮样式
    clearBtn->setStyleSheet("background-color: #FF6347; color: white;");

    // 添加示例数据
    addAlertItem("识别到人脸", "待处理!", QPixmap(":/image/sunny.png"));
    addAlertItem("有可疑车辆", "已处理!", QPixmap(":/image/tortoise.jpg"));
    // ... 添加其他条目

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(title);
    mainLayout->addWidget(list);
    mainLayout->addWidget(clearBtn);
    setLayout(mainLayout);

    connect(clearBtn,&QPushButton::clicked,this,&AlertWindow::clearLog);
}


void AlertWindow::addAlertItem(const QString &event, const QString &status, const QPixmap &thumb) {
    // 获取当前时间
    QString time = QTime::currentTime().toString("HH:mm:ss");

    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(QSize(0, 80)); // 固定每行高度

    AlertItem *widget = new AlertItem(time, event, status, thumb);
    list->addItem(item);
    list->setItemWidget(item, widget);


    // 自动滚动到顶部
    list->scrollToTop();
}

void AlertWindow::clearLog() {
    list->clear(); // 清空所有行
}
