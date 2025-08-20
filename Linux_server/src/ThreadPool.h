#ifndef __THREADPOOL_H
#define __THREADPOOL_H

#include "Thread.h"
#include "Task.h"
#include "TaskQueue.h"
#include <memory>
#include <vector>
using std::unique_ptr;
using std::vector;

namespace wd{

class ThreadPool{
public:
    ThreadPool(int,int);
    ~ThreadPool();
    void start();
    void stop();
    void addTask(Task &&);
    Task  getTask();
private:
    void doTask();
private:
    int _ThreadSize;
    int _queSzie;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskQue;
    bool _isExit;
};
}//end of namespace wd

#endif

