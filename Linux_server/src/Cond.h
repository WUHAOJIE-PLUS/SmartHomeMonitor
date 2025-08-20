#ifndef __COND_H
#define __COND_H

#include "Mutex.h"
#include "Noncopyable.h"
#include <pthread.h>

namespace wd{

class Condition
:Noncopyable
{
public:
    Condition(Mutex &mutex);
    ~Condition();

    void wait();
    void notifyOne();
    void notifyAll();
private:
    pthread_cond_t _cond;
    Mutex &_mutex;

};

}//end of namespace wd

#endif

