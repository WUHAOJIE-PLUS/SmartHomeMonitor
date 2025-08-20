#include "TcpCon.h"

#include <QTcpSocket>
#include <QDebug>
#include <QByteArray>

TCPConnection::TCPConnection(QWidget *parent)
: QWidget(parent)
{
    //1、创建用于通信的QTcpSocket的对象 （socket_fd）
    m_sock = new QTcpSocket(this);

    //当服务端发送数据后，就会触发客户端的readyREAD事件
    connect(m_sock, &QTcpSocket::readyRead, this, &TCPConnection::parseIncomingData);

    //6、连接信号与槽，连接断开的时候，需要做哪些后续的处理
    connect(m_sock, &QTcpSocket::disconnected, [=]{
        //关闭监听的对象
        m_sock->close();
        //清理QTcpSocket的资源
        m_sock->deleteLater();
        //将窗口关闭
        close();
    });

    //2、连接到指定的服务器
    m_sock->connectToHost("172.16.32.133", 8000);

    connect(m_sock,&QTcpSocket::connected,[=]{
        qDebug() << "已成功连接上服务器";
    });
}

TCPConnection::~TCPConnection()
{
}


void TCPConnection::sendPacket(int type, int length, const QString& data) {
    // 转换字节序 //主机字节序转网络字节序
    qint32 netType = qToBigEndian(static_cast<qint32>(type));
    qint32 netLength = qToBigEndian(static_cast<qint32>(length));

    // 一次性写入头部
    m_sock->write(reinterpret_cast<const char*>(&netType), sizeof(netType));
    m_sock->write(reinterpret_cast<const char*>(&netLength), sizeof(netLength));

    // 写入数据体
    QByteArray utf8Data = data.toUtf8();
    m_sock->write(utf8Data.constData(), utf8Data.size());
    m_sock->flush(); // 确保发送
}


void TCPConnection::parseIncomingData() {
    while (m_sock->bytesAvailable() >= 8) { // 等待完整头部
        // 读取头部
        qint32 netType, netLength;
        m_sock->read((char*)&netType, 4);
        m_sock->read((char*)&netLength, 4);

        // 字节序转换
        int type = qFromBigEndian(netType);
        int length = qFromBigEndian(netLength);

        // 等待完整数据体
        if (m_sock->bytesAvailable() < length) return;

        // 读取并处理数据
        QByteArray data = m_sock->read(length);
        qDebug() << "data: " << data;
        handlePacket(type, data); // 根据类型分发处理
    }
}

void TCPConnection::handlePacket(int type, const QByteArray& data) {
    switch (type) {
    case TASK_TYPE_REGISTER_SECTION1_RESP_OK:
        emit handleRegistre1_OK(data);
        break;
    case TASK_TYPE_REGISTER_SECTION1_RESP_ERROR:
        emit handleRegistre1_ERROR();
        break;
    case TASK_TYPE_REGISTER_SECTION2_RESP_OK:
        emit handleRegistre2_OK();
        break;
    case TASK_TYPE_REGISTER_SECTION2_RESP_ERROR:
        emit handleRegistre2_ERROR();
        break;
    case TASK_TYPE_LOGIN_SECTION1_RESP_OK:
        emit handleLogin1_OK(data);
        break;
    case TASK_TYPE_LOGIN_SECTION1_RESP_ERROR:
        emit handleLogin1_ERROR();
        break;
    case TASK_TYPE_LOGIN_SECTION2_RESP_OK:
        emit handleLogin2_OK();
        break;
    case TASK_TYPE_LOGIN_SECTION2_RESP_ERROR:
        emit handleLogin2_ERROR();
        break;
    }
}
