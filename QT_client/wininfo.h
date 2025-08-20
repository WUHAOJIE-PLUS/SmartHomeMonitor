#ifndef WININFO_H
#define WININFO_H

#include <QWidget>
#include <QTableWidget>

class WinInfo : public QWidget
{
    Q_OBJECT
public:
    explicit WinInfo(QWidget *parent = nullptr);
    void IninUI();
    void addLogEntry(const QString &message);

private slots:
    void clearLog();
signals:
private:
    QTableWidget *logTable;
};

#endif // WININFO_H
