#ifndef __TASK_H
#define __TASK_H

#include <functional>
using std::function;

namespace wd{
using Task = function<void()>;
}//end of namespace wd

#endif

