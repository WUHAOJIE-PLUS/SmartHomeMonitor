#include "Configuration.h"
#include <fstream>
#include <sstream>

namespace wd{

Configuration *Configuration::getInstance(){
    pthread_once(&_once,init);
    return _pInstance;
}


void Configuration::init(){
    _pInstance = new Configuration();
    atexit(destroy);
}
void Configuration::destroy(){
    if(_pInstance){
        delete _pInstance;
        _pInstance = nullptr;
    }

}

Configuration::Configuration()
:client()
{
    cout << "Configuration()" << endl;
    readConfiguration();
    iniMySQL();
}

Configuration::~Configuration(){
    cout << "~Configuration()" << endl;
}

void Configuration::readConfiguration(){
    
    string configfile("../conf/server.conf");
    ifstream ifs(configfile);
    if(ifs.good()){

        string line;
        while(getline(ifs,line)){
            istringstream iss(line);
            string key;
            string value;
            iss >> key >> value;
            _confileMap[key] = value;
        }
        cout << "reading config file " << configfile << " is over" << endl;

    }else{
        cout << "reading config file " << configfile << " error" << endl;
    }
}

bool Configuration::iniMySQL(){

    if(!client.connect(_confileMap["db_host"],_confileMap["db_user"],_confileMap["db_passwd"],_confileMap["db_name"],0)){
        cout << "数据库连接失败" << endl;
        return false;
    }
        cout << "数据库连接成功" << endl;
    return true;
    
}

MySQLClient& Configuration::getDBClient() {
        return client;
    }

map<string,string> & Configuration::getConfigMap(){
    return _confileMap;
}

void Configuration::display()const{
    for(auto &elem: _confileMap){
        cout << elem.first << "---->" << elem.second << endl;
    }
    cout << endl;
}

Configuration * Configuration::_pInstance = nullptr;
pthread_once_t Configuration::_once = PTHREAD_ONCE_INIT;


}//end of namespace wd
