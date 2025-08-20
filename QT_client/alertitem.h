#ifndef ALERTITEM_H
#define ALERTITEM_H

#include <QWidget>

class AlertItem : public QWidget
{
    Q_OBJECT
public:
      explicit AlertItem(const QString &time, const QString &event, const QString &status, const QPixmap &thumbnail, QWidget *parent = nullptr);

signals:
};

#endif // ALERTITEM_H
