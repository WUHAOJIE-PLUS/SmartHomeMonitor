#include "TaskQueue.h"

namespace wd{

TaskQueue::TaskQueue(int queSize)
    :_queSize(queSize)
     ,_que()
     ,_mutex()
     ,_cond(_mutex)
     ,_flag(true)
{
}

TaskQueue::~TaskQueue(){
}

void TaskQueue::push(ElemType &&ptask){


    //autolock是一个局部对象，当push函数
    //执行结束时，一定会销毁，在销毁时就会解锁
    MutexLockGuard autolock(_mutex);

    while(full()){
        _cond.wait();
    }
    _que.push(std::move(ptask));

    _cond.notifyOne();


}

ElemType TaskQueue::pop(){
    MutexLockGuard autolock(_mutex);
    while(empty() && _flag){
        _cond.wait();
    }
    if(_flag){
    ElemType ptask = _que.front();//finction<void()> ptask = bind(&MyTask::process,MyTask());
    _que.pop();

    _cond.notifyOne();
    return ptask;

    }else{
        return nullptr;
    }
}

bool TaskQueue::empty(){
    return _que.empty();
}

bool TaskQueue::full(){
    return _que.size() == _queSize;
}  

void TaskQueue::wakeup(){
    _flag = false;
    _cond.notifyAll();
}

}//end of namespace wd
