#ifndef __TASKQUEUE_H
#define __TASKQUEUE_H

#include <queue>
#include "Mutex.h"
#include "Cond.h"
#include "Task.h"
#include "MutexLockGuard.h"

using std::queue;

namespace wd{
using ElemType = Task;

class TaskQueue{
public:
    TaskQueue(int);
    ~TaskQueue();
    void push(ElemType &&);
    ElemType pop();
    bool empty();
    bool full();
    void wakeup();

private:
    int _queSize;
    queue<ElemType> _que;
    Mutex _mutex;
    Condition _cond;
    bool _flag;
};

}//end of namespace wd

#endif

