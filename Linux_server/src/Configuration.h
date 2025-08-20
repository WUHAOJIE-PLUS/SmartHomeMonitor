#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include <iostream>
#include <string>
#include <map>
#include "MySQLClient.h"

using namespace std;

namespace wd{

class Configuration{
public:

    static Configuration *getInstance();
    map<string,string> &getConfigMap();
    void display()const;
    MySQLClient& getDBClient();

private:
    Configuration();
    ~Configuration();
    static void init();
    static void destroy();
    
    void readConfiguration();
    bool iniMySQL();

    Configuration(const Configuration &rhs) = delete;
    Configuration& operator=(const Configuration &rhs) = delete;

private:
    static Configuration* _pInstance;
    static pthread_once_t _once;
    map<string,string> _confileMap;
    wd::MySQLClient client;
};

}//end of namespace wd
#endif

