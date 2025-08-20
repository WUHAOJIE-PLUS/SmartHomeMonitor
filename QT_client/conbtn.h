#ifndef CONBTN_H
#define CONBTN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "http.h"

class ConBtn : public QWidget
{
    Q_OBJECT
public:
    explicit ConBtn(QWidget *parent = nullptr);
    void setlayout();
    void applyStyles();
    void setConnect();

private slots:
    void onUpClicked();
    void onDownClicked();
    void onLeftClicked();
    void onRightClicked();
    void onUpLeftClicked();
    void onUpRightClicked();
    void onDownLeftClicked();
    void onDownRightClicked();
    void onCenterClicked();

    void onZoomClicked_add();
    void onZoomClicked_muins();
    void onFocusClicked_add();
    void onFocusClicked_muins();
    void onLightClicked_add();
    void onLightClicked_muins();

    void change_voice_icon();

signals:
    void sendInfo(const QString &);
    void voiceChange_mute();
    void voiceChange();
private:
    QPushButton *btnUp;
    QPushButton *btnDown;
    QPushButton *btnLeft;
    QPushButton *btnRight;
    QPushButton *btnUpLeft;
    QPushButton *btnUpRight;
    QPushButton *btnDownLeft;
    QPushButton *btnDownRight;
    QPushButton *btnCenter;

    QPushButton *Zoom_add;
    QPushButton *Zoom_minus;
    QPushButton *Focus_add;
    QPushButton *Focus_minus;
    QPushButton *ligth_add;
    QPushButton *light_minus;

    QPushButton *voice_btn;



    QLabel *zoom;
    QLabel *focus;
    QLabel *light;

    QLabel *label;
    HttpReq *httpreq;

    bool voice_icon;//默认为true，表示声音打开，false，表示声音关闭
};

#endif // CONBTN_H
