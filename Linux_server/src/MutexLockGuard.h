#ifndef __MUTEXLOCKGUARD_H
#define __MUTEXLOCKGUARD_H
#include "Mutex.h"

namespace wd{

class MutexLockGuard{
public:
    MutexLockGuard(Mutex &);
    ~MutexLockGuard();
private:
    Mutex &_mutex;
};

}//end of namespace wd

#endif

