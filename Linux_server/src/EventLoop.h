#ifndef __EVENTLOOP_H
#define __EVENTLOOP_H

#include "TcpConnection.h"
#include "Mutex.h"
#include "MutexLockGuard.h"
#include <memory>
#include <functional>
#include <map>
#include <vector>
#include <header.h>

using std::shared_ptr;
using std::function;
using std::map;
using std::vector;

namespace wd{
using TcpConnectionPtr=shared_ptr<TcpConnection>;
using TcpConnecitonCallback=function<void(TcpConnectionPtr)>;
using Functor = function<void()>;

class Acceptor;//类的前向声明，不需要头文件

class EventLoop{
public:
    EventLoop(Acceptor&);
    ~EventLoop();
    void loop();
    void unloop();//退出循环，要与loop函数运行在不同线程
    void runInLoop(Functor &&cb);
    void setAllCallbacks(TcpConnecitonCallback &&cb1,
                         TcpConnecitonCallback &&cb2,
                         TcpConnecitonCallback &&cb3);
private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int);

    int createEpollFd();
    void addEpollReadEvent(int);
    void delEpollReadEvent(int);

   int createEventfd();
   void handleReadEvent();
   void wakeup();
   void doPendingFunctors();
private:
    int _epfd;
    int _eventfd;//通知
    bool _isLooping;
    Acceptor &_acceptor;
    //vector要比数组性能更好
    vector<struct epoll_event> _evtArr;
    map<int,TcpConnectionPtr> _conns;
    TcpConnecitonCallback _onConnection;
    TcpConnecitonCallback _onMessage;
    TcpConnecitonCallback _onClose;

    //vector是因为多个客户端同时输入信息时，都需要通知send，但中间有一段距离，
    //怕数据覆盖，所以一接收到就先存放到vector中，后面再集中通知send
    vector<Functor> _pendingFunctors;
    Mutex _mutex;
};

}//end of namespace wd

#endif

