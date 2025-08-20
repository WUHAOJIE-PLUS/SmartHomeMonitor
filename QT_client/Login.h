#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "TcpCon.h"

class LoginTab : public QWidget
{
    Q_OBJECT

public:
    LoginTab(QWidget *parent = nullptr);
    ~LoginTab();
    void setlayout();
    void applyStyles();
    void setConnect();

signals:
    void Login_end();

private slots:
    //客户端发送处理
    void UserRegistreSection();
    void UserLoginSection();

    //客户端接收处理
    void handleLogin1_OK(const QByteArray& data);
    void handleLogin1_ERROR();
    void handleLogin2_OK();
    void handleLogin2_ERROR();

    void handleRegistre1_OK(const QByteArray& data);
    void handleRegistre1_ERROR();
    void handleRegistre2_OK();
    void handleRegistre2_ERROR();
private:
    QPushButton *Login_btn;
    QPushButton *Registry_btn;
    QLineEdit *username_line;
    QLineEdit *password_line;

    TCPConnection *net_fd;
    QString username;
    QString password;
};
#endif // LOGIN_H
