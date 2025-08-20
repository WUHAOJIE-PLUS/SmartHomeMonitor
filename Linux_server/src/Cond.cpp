#include "Cond.h"

namespace wd{
Condition::Condition(Mutex &mutex)
:_mutex(mutex)
{
    pthread_cond_init(&_cond,nullptr);
}

Condition::~Condition(){
    pthread_cond_destroy(&_cond);
}

void Condition::wait(){
    //因为没有拷贝和赋值函数，所以需要找到原始_mutex的地址
    pthread_cond_wait(&_cond,_mutex.getMutexLockPtr());

}

void Condition::notifyOne(){
    pthread_cond_signal(&_cond);

}

void Condition::notifyAll(){
    pthread_cond_broadcast(&_cond);
}

}//end of namespace wd
