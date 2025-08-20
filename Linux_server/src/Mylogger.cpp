#include "Mylogger.h"
#include "Configuration.h"

namespace wd{
void Logger::warn(const char * file, int line, const char * func, const char * msg){//这个warn是Logger类的成员函数
     _root.warn("[%s:%d] [%s] :%s", file, line, func, msg);//这个warn是Category类的成员函数
}
void Logger::error(const char * file, int line, const char * func, const char * msg){
     _root.error("[%s:%d] [%s] :%s", file, line, func, msg);

}
void Logger::debug(const char * file, int line, const char * func, const char * msg){
     _root.debug("[%s:%d] [%s] :%s", file, line, func, msg);
}
void Logger::info(const char * file, int line, const char * func, const char * msg){
     _root.info("[%s:%d] [%s] :%s", file, line, func, msg);
}

Logger *Logger:: getInstance(){
    pthread_once(&_once,init);
    return _pInstance;
}

void Logger::init(){
    _pInstance = new Logger();
    atexit(destroy);
}

void Logger::destroy(){
    if(_pInstance)
    {
        delete _pInstance;
        _pInstance = nullptr;
        Category::shutdown();
    }
}
Logger::Logger(){
    //日志的格式
    PatternLayout *ppl = new PatternLayout();
    ppl->setConversionPattern("%d %c [%p]:%m%n");

    PatternLayout *ppl2 = new PatternLayout();
    ppl2->setConversionPattern("%d %c [%p]:%m%n");

    //日志的目的地
    OstreamAppender *poa = new OstreamAppender("console",&cout);
    poa->setLayout(ppl);

    string logfile = wd::Configuration::getInstance()->getConfigMap()["log_file"];
    FileAppender *pfl = new FileAppender("file",logfile);
    pfl->setLayout(ppl2);

    //日志记录器
    _root.addAppender(poa);
    _root.addAppender(pfl);

    //日志过滤器
    _root.setPriority(Priority::DEBUG);

}
Logger:: ~Logger(){


}

Logger * Logger::_pInstance = nullptr;
Category &Logger::_root = Category::getRoot();
pthread_once_t Logger::_once = PTHREAD_ONCE_INIT;
}//end of namespace wd
