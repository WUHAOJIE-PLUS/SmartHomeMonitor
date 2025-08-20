#ifndef ALERTWINDOW_H
#define ALERTWINDOW_H

#include <QWidget>
#include <QListWidget>
#include "alertitem.h"
class AlertWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AlertWindow(QWidget *parent = nullptr);

    void addAlertItem(const QString &event, const QString &status, const QPixmap &thumb);

private slots:

    void clearLog();
signals:

private:
    QListWidget *list;
};

#endif // ALERTWINDOW_H
