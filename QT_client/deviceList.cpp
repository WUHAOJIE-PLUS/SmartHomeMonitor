#include "deviceList.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QLabel>
#include <QDebug>

DeviceList::DeviceList(QWidget *parent)
    : QWidget{parent}
{
    InitUI();
    Connect();
}

void DeviceList::InitUI()
{
    this->setFixedWidth(200);
    QLabel *listLabel = new QLabel("设备列表");
    listLabel->setAlignment(Qt::AlignCenter);
    listLabel->setStyleSheet("background-color: black;; color: white;");

    // 创建设备树
    deviceTree = new QTreeWidget(this);
    deviceTree->setHeaderHidden(true);

    // 添加设备树项
    QTreeWidgetItem *defaultGroup = new QTreeWidgetItem(deviceTree);
    defaultGroup->setText(0, "默认分组");
    defaultGroup->setData(0, Qt::UserRole, "group");

    QTreeWidgetItem *networkVideo = new QTreeWidgetItem(defaultGroup);
    networkVideo->setText(0, "网络视频");
    networkVideo->setData(0, Qt::UserRole, "category");

    // 添加设备项
    addDeviceItem(networkVideo, "网络视频1", "cam1");
    addDeviceItem(networkVideo, "网络视频2", "cam2");
    addDeviceItem(networkVideo, "网络视频3", "cam3");
    addDeviceItem(networkVideo, "网络视频4", "cam4");

    deviceTree->addTopLevelItem(defaultGroup);
    deviceTree->expandAll();

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setSpacing(0);
    vboxLayout->addWidget(listLabel);
    vboxLayout->addWidget(deviceTree);
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(vboxLayout);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground);
}

void DeviceList::addDeviceItem(QTreeWidgetItem *parent, const QString &name, const QString &id)
{
    QTreeWidgetItem *device = new QTreeWidgetItem(parent);
    device->setText(0, name);
    device->setData(0, Qt::UserRole, "device");
    device->setData(0, Qt::UserRole + 1, id); // 存储设备ID

    QTreeWidgetItem *mainStream = new QTreeWidgetItem(device);
    mainStream->setText(0, "主码流");
    mainStream->setData(0, Qt::UserRole, "stream");
    mainStream->setData(0, Qt::UserRole + 1, id + "_main");

    QTreeWidgetItem *subStream = new QTreeWidgetItem(device);
    subStream->setText(0, "子码流");
    subStream->setData(0, Qt::UserRole, "stream");
    subStream->setData(0, Qt::UserRole + 1, id + "_sub");
}

void DeviceList::Connect()
{
    // 连接设备树双击信号
    connect(deviceTree, &QTreeWidget::itemDoubleClicked, this, &DeviceList::onItemDoubleClicked);
}


void DeviceList::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    //可以加switch
    QString type = item->data(0, Qt::UserRole).toString();
    QString id = item->data(0, Qt::UserRole + 1).toString();

    if (type == "device") {
        // 双击设备时切换展开/折叠状态
        item->setExpanded(!item->isExpanded());
    }

    if (id == "cam1" || id == "cam2" || id == "cam3" || id == "cam4")  {
        // 双击设备时切换展开/折叠状态
        item->setExpanded(!item->isExpanded());
    }

    if (id  == "cam1_main") {
        emit cam_main(1);
    }
    if (id  == "cam2_main") {
        emit cam_main(2);
    }
    if (id  == "cam3_main") {
        emit cam_main(3);
    }
    if (id  == "cam4_main") {
        emit cam_main(4);
    }

    if (id  == "cam1_sub") {
        emit cam_sub(1);
    }
    if (id  == "cam2_sub") {
        emit cam_sub(2);
    }
    if (id  == "cam3_sub") {
        emit cam_sub(3);
    }
    if (id  == "cam4_sub") {
        emit cam_sub(4);
    }
    emit sendInfo( "点击了" + id);
}

