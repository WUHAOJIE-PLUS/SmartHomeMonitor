#ifndef TCPCON_H
#define TCPCON_H

#include <QWidget>
#include <QTcpSocket>
#include "Message.h"

class TCPConnection
:public QWidget
{
    Q_OBJECT

public:
    TCPConnection(QWidget *parent = nullptr);
    ~TCPConnection();
    QTcpSocket * get_scoket(){ return m_sock;}
    void sendPacket(int type, int length, const QString& data);
    void handlePacket(int type, const QByteArray& data);
private slots:
    void parseIncomingData();
signals:
    void handleLogin1_OK(const QByteArray& data);
    void handleLogin1_ERROR();
    void handleLogin2_OK();
    void handleLogin2_ERROR();

    void handleRegistre1_OK(const QByteArray& data);
    void handleRegistre1_ERROR();
    void handleRegistre2_OK();
    void handleRegistre2_ERROR();
private:
    QTcpSocket *m_sock;
};
#endif // TCPCON_H
