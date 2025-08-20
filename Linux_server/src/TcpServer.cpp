#include "TcpServer.h"

namespace wd{

TcpServer::TcpServer(unsigned short port,const string &ip)
    :_acceptor(port,ip)
     ,_loop(_acceptor)
{}
void TcpServer::setAllCallbacks(TcpConnecitonCallback &&cb1,
                                TcpConnecitonCallback &&cb2,
                                TcpConnecitonCallback &&cb3){

    _loop.setAllCallbacks(std::move(cb1),
                          std::move(cb2),
                          std::move(cb3));
}                                 
void TcpServer::start(){
    _acceptor.ready();
    _loop.loop();
}

//stop函数要与start函数运行在不同的线程
void TcpServer::stop(){
    _loop.unloop();
}

}//end of namespace wd
