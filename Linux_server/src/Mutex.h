#ifndef __MUTEX_H
#define __MUTEX_H

#include "Noncopyable.h"
#include <pthread.h>

namespace wd{

class Mutex
:Noncopyable
{
public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
    pthread_mutex_t* getMutexLockPtr();
private:
    pthread_mutex_t _mutex;
};

}//end of namespace wd

#endif

