#ifndef CTITLEBAR_H
#define CTITLEBAR_H

#include <QWidget>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

class CTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit CTitleBar(QWidget *parent = nullptr);
private:
    void initUI();
signals:
    void sigClose();
    void sendInfo(const QString &);

private slots:
    void onClickedSlot();
private:
    QLabel * _plogoLabel;
    QLabel * _ptitleTextLabel;
    QLabel * timeLabel;

    QPushButton * _ptoolButton;
    QPushButton * _pfileButton;
    QPushButton * _pmapButton;
    QPushButton * _pveidoButton;
    QPushButton * _pmonitorButton;

    QPushButton * _pminButton;
    QPushButton * _pmaxButton;
    QPushButton * _pcloseButton;
};

#endif // CTITLEBAR_H
