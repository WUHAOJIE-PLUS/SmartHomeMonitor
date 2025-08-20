#include "MutexLockGuard.h"
#include <iostream>

using std::endl;
using std::cout;

namespace wd{

MutexLockGuard::MutexLockGuard(Mutex &mutex)
:_mutex(mutex){
    _mutex.lock();
    //cout << "MutexLockGuard lock" << endl;
}

MutexLockGuard::~MutexLockGuard(){
    _mutex.unlock();
    //cout << "MutexLockGuard unlock" << endl;
}

}//end of namespace wd
