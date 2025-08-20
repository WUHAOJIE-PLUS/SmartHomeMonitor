#ifndef __THREAD_H
#define __THREAD_H

#include "Noncopyable.h"
#include <pthread.h>
#include <functional>

using std::function;
using std::bind;

namespace wd{

using ThreadCallback=function<void()>;

class Thread
:Noncopyable
{
public:
    Thread(ThreadCallback &&);
    virtual ~Thread();
    void start();
    void join();
private:
    static void * start_routine(void *);
private:
    pthread_t _pthid;
    bool _isRunning;
    ThreadCallback _cb;
};

}//end of namespace wd
#endif

