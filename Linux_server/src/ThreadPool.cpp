#include "ThreadPool.h"
#include <memory>
#include <iostream>
#include <unistd.h>

using std::unique_ptr;
using std::endl;
using std::cout;
using std::bind;

namespace wd{
ThreadPool::ThreadPool(int ThreadSize,int queSize)
    :_ThreadSize(ThreadSize)
     ,_queSzie(queSize)
     ,_threads()
     ,_taskQue(_queSzie)
     ,_isExit(false)
{
    //提前开辟空间
    _threads.reserve(_ThreadSize);
}


ThreadPool::~ThreadPool(){}

void ThreadPool::start(){
    int num = _ThreadSize;
    while(num--){
        unique_ptr<Thread> up(new Thread(bind(&ThreadPool::doTask,this)));//注册回调函数
        _threads.push_back(std::move(up));
    }

    for(auto &elem:_threads){
        elem->start();
    }
}

void ThreadPool::stop(){
    while(!_taskQue.empty()){
        sleep(1);
    }

    _isExit = true;//线程池关闭
    _taskQue.wakeup();

    cout << endl << "threadpool is starting join sub thread" << endl;
    for(auto &elem: _threads){
        elem->join();
    }

}

void ThreadPool::addTask(Task &&task){
    if(task){
        _taskQue.push(std::move(task));
    }
}

Task ThreadPool::getTask(){
    return _taskQue.pop();

}

void ThreadPool::doTask(){

    while(!_isExit){
        //Task task = _taskQue.pop();
        Task task = getTask();
        if(task){
            task();//执行回调函数
        }else{
            cout << "nullptr == task" << endl;
        }
    }
}

}//end of namespace wd
