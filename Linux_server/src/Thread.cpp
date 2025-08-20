#include "Thread.h"
#include <stdio.h>
#include <string.h>


namespace wd{

Thread::Thread(ThreadCallback &&cb)
    :_pthid(0)
     ,_isRunning(false)//初始未运行
     ,_cb(std::move(cb))
{
}

Thread::~Thread(){}

void Thread::start(){
    if(!_isRunning){
        int ret = pthread_create(&_pthid,nullptr,start_routine,this);
        if(ret != 0){
            fprintf(stderr,"pthread_create: %s\n",strerror(ret));
            return;
        }
        _isRunning = true;
    }
}

void Thread::join(){
    if(_isRunning){
        pthread_join(_pthid,nullptr);
        _isRunning = false;
    }
}

void *Thread::start_routine(void *arg){
    Thread *thread = static_cast<Thread *>(arg);
    if(thread){
        thread->_cb();//回调函数的执行
    }
    return nullptr;
}
}
