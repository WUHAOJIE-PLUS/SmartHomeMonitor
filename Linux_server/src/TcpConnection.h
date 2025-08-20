#ifndef __TCPCONNECTION_H
#define __TCPCONNECTION_H

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"
#include "Message.h"
#include <header.h>
#include <string>
#include <memory>
#include <functional>

using std::shared_ptr;
using std::string;
using std::function;

namespace wd{
class EventLoop;
class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(TcpConnectionPtr)>;

class TcpConnection
    //辅助类，为了在成员函数内部通过this直接获取本对象的智能指针shared_ptr
    :public std::enable_shared_from_this<TcpConnection> 
{
public:
    TcpConnection(int fd,EventLoop *loop);
    ~TcpConnection();

    string receive();
    int readPacket(Packet &packet);
    void sendPacket(int type, const std::string& data);
    void send(const string &);
    void sendInLoop(const string &msg);
    void sendInLoop(const TLV &data);


    void shutdown();
    bool isClose();
    string toString();
    void setAllCallbacks(const TcpConnectionCallback &cb1,
                         const TcpConnectionCallback &cb2,
                         const TcpConnectionCallback &cb3);
    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

    //用户名
    void setUsername(const std::string& username) { _username = username; }
    string getUsername() const { return _username; }

    //密文
    void setStoredPassword(const std::string& StoredPassword) { _StoredPassword = StoredPassword; }
    string getStoredPassword() const { return _StoredPassword; }
    
    //盐值
    void setSalt(const std::string& salt) { _salt = salt; }
    string getSalt() const { return _salt; }


private:
    InetAdress getLoaclAddress();
    InetAdress getPeerAddress();
private:
    Socket _socket;
    SocketIO _sockIO;
    InetAdress _localAddress;
    InetAdress _peerAddress;
    bool _isShutdownWrite;//是否主动关闭连接
    EventLoop *_loop;


    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;

    string _username;
    string _StoredPassword;
    string _salt;
};

}//end of namespace wd

#endif

