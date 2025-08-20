#include "SmartHomeMonitorServer.h"
#include "Configuration.h"
#include <unistd.h>
#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;


int main(void)
{
    wd::Configuration * pconf = wd::Configuration::getInstance();
    wd::SmartHomeMonitorServer  server(
            atoi(pconf->getConfigMap()["thread_num"].c_str()), 
            atoi(pconf->getConfigMap()["task_num"].c_str()),
            atoi(pconf->getConfigMap()["port"].c_str()));

    server.start();

	return 0;
}

