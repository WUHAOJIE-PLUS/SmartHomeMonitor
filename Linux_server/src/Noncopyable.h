#ifndef __NONCOPYABLE_H
#define __NONCOPYABLE_H

namespace wd{
class Noncopyable{
    //定义了protected构造函数的类为抽象类
protected:
    Noncopyable(){};
    ~Noncopyable(){};
    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator=(const Noncopyable &) = delete;
};
}//end of namespace wd

#endif

