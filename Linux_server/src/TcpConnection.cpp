#include "TcpConnection.h"
#include "EventLoop.h"
#include <sstream>
#include <iostream>

using namespace std;

namespace wd{

TcpConnection::TcpConnection(int fd,EventLoop *loop)
    :_socket(fd)
     ,_sockIO(fd)
     ,_localAddress(getLoaclAddress())
     ,_peerAddress(getPeerAddress())
     ,_isShutdownWrite(false)
     ,_loop(loop)
{

}

TcpConnection::~TcpConnection(){
    if(!_isShutdownWrite){
        shutdown();
    }
}

string TcpConnection::receive(){

    char buff[65536] = {0};
    //cout << "=======receive before========" << endl;
    _sockIO.readline(buff,sizeof(buff));
    //cout << "=======receive after========" << endl;
    return string(buff);
}

int TcpConnection::readPacket(Packet &packet){
    return _sockIO.readPacket(packet);
}

void TcpConnection::send(const string &msg){
    _sockIO.sendn(msg.c_str(), msg.size());
}
 
void TcpConnection::sendInLoop(const string &msg){
    if(_loop){
        //注册回调函数
        _loop->runInLoop(std::bind(&TcpConnection::send,this,msg));
    }
}

//将TLV经过处理，转到普通的发送
void TcpConnection::sendInLoop(const TLV &data){
    int tlvlen = 8 + data.length;//整个TLV的大小
    string msg;
    msg.assign((const char*)&data,tlvlen);//将整个TLV的内容（出文本），拷贝到字符串msg
    sendInLoop(msg);//发送字符串,转到普通的发送
}

void TcpConnection::sendPacket(int type, const std::string& data){
   _sockIO.sendPacket(type,data);
}
void TcpConnection::shutdown(){
    if(!_isShutdownWrite){
        _isShutdownWrite = true;
        _socket.shutdownWrite();
    }
}

//判断一个连接是否断开
bool TcpConnection::isClose(){
    char buff[20] = {0};
    return  _sockIO.recvPeek(buff,sizeof(buff)) == 0;
}

string TcpConnection::toString(){
    std::ostringstream oss;
    oss << "tcp: " << _localAddress.ip() << ":" << _localAddress.port()
        << " --> " << _peerAddress.ip() << ":" << _peerAddress.port();
    return oss.str();
}

InetAdress TcpConnection::getLoaclAddress(){
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t len = sizeof(addr);
    int ret = getsockname(_socket.fd(), (struct sockaddr*)&addr, &len);
    if(ret < 0) {
        perror("getsockname");
    }
    return InetAdress(addr);
}

InetAdress TcpConnection::getPeerAddress(){
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t len = sizeof(addr);
    int ret = getpeername(_socket.fd(), (struct sockaddr*)&addr, &len);
    if(ret < 0) {
        perror("getpeername");
    }
    return InetAdress(addr);
}

void TcpConnection::setAllCallbacks(const TcpConnectionCallback &cb1,
                    const TcpConnectionCallback &cb2,
                    const TcpConnectionCallback &cb3){
    _onConnection = cb1;
    _onMessage = cb2;
    _onClose = cb3;

}

void TcpConnection::handleNewConnectionCallback(){

    if(_onConnection){
        _onConnection(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback(){
    if(_onMessage){
        _onMessage(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback(){
    if(_onClose){
        _onClose(shared_from_this());
    }

}

}//end of namespace wd
