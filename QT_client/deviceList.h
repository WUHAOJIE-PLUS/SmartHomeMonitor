#ifndef DEVICELIST_H
#define DEVICELIST_H

#include <QWidget>
#include <QTreeWidget>

class DeviceList : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceList(QWidget *parent = nullptr);

private:
    void InitUI();
    void Connect();
    void addDeviceItem(QTreeWidgetItem *parent, const QString &name, const QString &id);
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onItemExpanded(QTreeWidgetItem *item);
    void onItemCollapsed(QTreeWidgetItem *item);
    void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
signals:
    void deviceSelected(QString);
    void streamSelected(QString);

    void cam_main(int);
    void cam_sub(int);

    void sendInfo(const QString &);

private:
    QTreeWidget *deviceTree;
};

#endif // DEVICELIST_H
