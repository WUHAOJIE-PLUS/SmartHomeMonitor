#include <iostream>
#include "Configuration.h"
using std::cout;
using std::endl;

void test(){

    wd::Configuration *conf = wd::Configuration::getInstance();
    conf->display();

}

int main()
{
    test();
    return 0;
}

