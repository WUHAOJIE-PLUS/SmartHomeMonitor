#ifndef __MYLOGGER_H
#define __MYLOGGER_H

#include <iostream>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/SimpleLayout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/Category.hh>
#include<log4cpp/Priority.hh>
using namespace std;
using namespace log4cpp;

#define SHMS_LOG_WARN(msg) Logger::getInstance()->warn(__FILE__,__LINE__,__FUNCTION__,msg)
#define SHMS_LOG_ERROR(msg) Logger::getInstance()->error(__FILE__,__LINE__,__FUNCTION__,msg)
#define SHMS_LOG_DEBUG(msg) Logger::getInstance()->debug(__FILE__,__LINE__,__FUNCTION__,msg)
#define SHMS_LOG_INFO(msg) Logger::getInstance()->info(__FILE__,__LINE__,__FUNCTION__,msg)


namespace wd{
class Logger{
public:
    void warn(const char * file, int line, const char * func, const char * msg);
    void error(const char * file, int line, const char * func, const char * msg);
    void debug(const char * file, int line, const char * func, const char * msg);
    void info(const char * file, int line, const char * func, const char * msg);

    static Logger * getInstance();

private:
    Logger();
    ~Logger();
    static void init();
    static void destroy();

    Logger(const Logger &rhs) = delete;
    Logger & operator=(const Logger &rhs) = delete;
    static Logger * _pInstance;
    static Category &_root;
    static pthread_once_t _once;
};
}//end of namespace wd
 
#endif
