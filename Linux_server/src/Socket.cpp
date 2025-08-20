#include "Socket.h"

namespace wd{
//socket_fd
Socket::Socket(){
    _fd = socket(AF_INET,SOCK_STREAM,0);
    if(_fd < 0){
        perror("socket");
    }
}
//net_fd
Socket::Socket(int fd)
:_fd(fd)
{

}

Socket::~Socket(){
    close(_fd);
}

int Socket::fd()const{
    return _fd;
}

void Socket::shutdownWrite(){
    //主动断开连接是调用，只关闭写端
    shutdown(_fd,SHUT_WR);
}

}//end of namespace wd
