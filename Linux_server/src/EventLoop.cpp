#include "EventLoop.h"
#include "Acceptor.h"
#include <cerrno>    // for errno and EINTR
#include <unistd.h>  // for read()
#include <sys/eventfd.h>


namespace wd{

EventLoop::EventLoop(Acceptor &acceptor)
    :_epfd(createEpollFd())
     ,_eventfd(createEventfd())
     ,_isLooping(false)
     ,_acceptor(acceptor)
     ,_evtArr(1000)
{
         addEpollReadEvent(_acceptor.fd());
         addEpollReadEvent(_eventfd);
     }

EventLoop::~EventLoop(){
    close(_epfd);
    close(_eventfd);
}

void EventLoop::loop(){
    _isLooping = true;
    while(_isLooping){
        waitEpollFd();
    }
}

void EventLoop::unloop(){
    _isLooping = false;
}

void EventLoop::runInLoop(Functor &&cb){
    {
        //多线程环境下要加锁
        MutexLockGuard autolock(_mutex);
        _pendingFunctors.push_back(std::move(cb));
    }
    //通知IO线程发送数据
    wakeup();
} 

void EventLoop::setAllCallbacks(TcpConnecitonCallback &&cb1,
                                TcpConnecitonCallback &&cb2,
                                TcpConnecitonCallback &&cb3){
    //要表达完整的移动语义，就需要使用std::move
    //将左值(cb1,cb2,cb3)转换成右值
    //std::move的作用是显式将一个左值转换成右值
    _onConnection = std::move(cb1);
    _onMessage = std::move(cb2);
    _onClose = std::move(cb3);

}

void EventLoop:: waitEpollFd(){
    //epoll_wait
    int nready = epoll_wait(_epfd,_evtArr.data(),_evtArr.size(),5000);
    if(nready == -1 && errno == EINTR){
        return;
    }else if(nready == -1){
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }else if(nready == 0){
//        perror("epoll timeout\n");
    }else{
        //nready > 0
        for(int i = 0; i < nready; ++i){
            int fd = _evtArr[i].data.fd; 
            if(fd == _acceptor.fd()){
                //有新连接
                handleNewConnection();
            }else if(fd == _eventfd){
                handleReadEvent();
                doPendingFunctors();
            }else{
                //接收到消息
                handleMessage(fd);
            }
        }
    }

}

void EventLoop::handleNewConnection(){
    //获取新连接
    int net_fd = _acceptor.accept();
    //epoll添加对于net_fd的监听
    addEpollReadEvent(net_fd);
    //创建TcpConnection对象
    TcpConnectionPtr conn(new TcpConnection(net_fd,this));
    //设置三个函数对象
    conn->setAllCallbacks(_onConnection,_onMessage,_onClose);
    //添加到容器map
    _conns.insert(std::make_pair(net_fd,conn));
    //调用连接建立时的函数对象
    conn->handleNewConnectionCallback();
}

void EventLoop::handleMessage(int fd){
    //先通过fd查找到TcpConnection对象
    auto iter = _conns.find(fd);
    if(iter != _conns.end()){
        //判断连接是否断开
        bool isClosed = iter->second->isClose();
        if(isClosed){
            //连接断开的情况，调用连接断开时的函数对象
            iter->second->handleCloseCallback();
            //从epoll的监听红黑树上删除
            delEpollReadEvent(fd);
            //从map中删除
            _conns.erase(fd);
        }else{
            //发送消息过来了，开始调用消息到达时的函数对象
            iter->second->handleMessageCallback();
        }
    }
}


int EventLoop::createEpollFd(){
    int fd = epoll_create1(0);
    if(fd < 0){
        perror("epoll_create");
    }
    return  fd;
}

void EventLoop::addEpollReadEvent(int fd){
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
    if(ret < 0){
        perror("epoll_ctl");
    }
}

void EventLoop::delEpollReadEvent(int fd){
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd = fd;
    int ret = epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&ev);
    if(ret < 0){
        perror("epoll_ctl");
    }
}


int EventLoop::createEventfd(){
    int fd = eventfd(0,0);
    if(fd < 0){
        perror("eventfd");
    }
    return fd;
}

//处理内核计数器的值（清零）
void EventLoop::handleReadEvent(){
    uint64_t howmany = 0;
    int ret = read(_eventfd,&howmany,sizeof(howmany));
   // printf("\nhowmany: %ld\n",howmany);
    if(ret != sizeof(howmany)){
        perror("read");
    }
}

void EventLoop::wakeup(){
    uint64_t one = 1;
    int ret = write(_eventfd,&one,sizeof(one));
    if(ret != sizeof(one)){
        perror("write");
    }
}

void EventLoop::doPendingFunctors(){
//    printf(">> doPendingFunctors\n");

    vector<Functor> tmp;

    {
        //使用语句块，减小加锁的范围
      MutexLockGuard autolock(_mutex);  
      tmp.swap(_pendingFunctors);//O(1)
    }//这时_pendingFunctors容器为空

    //经过交换之后，_pendingFunctors被解放出来了，
    //其他的计算线程可以对他进行修改操作
    for(auto & f : tmp) {//多个计算线程
        //回调函数的执行
        f();//执行send
    }
}

}//end of namespace wd

