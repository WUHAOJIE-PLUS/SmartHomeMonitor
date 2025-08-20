#ifndef __SOCKETIO_H
#define __SOCKETIO_H

#include <header.h>
#include "Message.h"

namespace wd{

class SocketIO{
public:
    SocketIO(int);

    void sendPacket(int type, const std::string& data);
    int sendn(const char *buff,int len);
    int readn(char *buff,int len);
    int readline(char *buff,int maxlen);
    int readPacket(Packet &packet);
    int recvPeek(char *buff,int maxlen)const;
private:
    int _fd;
};

}//end of namespace wd

#endif

