#include "BusinessService.h"
#include "Configuration.h"
#include "Message.h"
#include "Mylogger.h"

#include <string>
#include <shadow.h>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

namespace wd{
//用户名存在，则从数据库中取出盐值发送给client
void UserLoginSection1::process(){
    cout << "login section 1 is processing" << endl;

    //执行用户登陆阶段1操作
    if(_packet.type == wd::TASK_TYPE_LOGIN_SECTION1){
        //消息内容为用户名
        string username = _packet.msg;

        // 添加错误处理
        if (username.empty()) {
            SHMS_LOG_DEBUG("Username is empty in section1");
            return;
        }

        //获取数据库的连接
        auto &db = Configuration::getInstance()->getDBClient();

        string sql = "SELECT salt,encrypt FROM Smart_User WHERE name = '" + username + "'"; 
        auto result = db.readOperationQuery(sql);

        if(result.empty()){
            //用户名出错
            SHMS_LOG_DEBUG("username does not exist");
            //告知对端有问题
            _conn->sendPacket(TASK_TYPE_LOGIN_SECTION1_RESP_ERROR,"ERROR");
            return;
        }

        //获取要发送给对端的salt
        //storedPassword为密文
        string salt = result[1][0];
        string storedPassword = result[1][1];


        cout << "保存用户名: " << username <<  endl;
        SHMS_LOG_INFO("username exist in TASK_TYPE_LOGIN_SECTION1");

        // 保存用户名和storedPassword到连接
        _conn->setUsername(username);
        _conn->setStoredPassword(storedPassword);

        //构造TLV，发送给对端(成功)
        _conn->sendPacket(TASK_TYPE_LOGIN_SECTION1_RESP_OK,salt);
    }

    cout << endl;
}

//查询数据库中的加密密文是否与client发送过来的encrypted相同
void UserLoginSection2::process(){

    cout << "login section 2 is processing" << endl;
    //执行用户登录阶段2操作

    if(_packet.type == wd::TASK_TYPE_LOGIN_SECTION2){
        // 从连接中获取用户名
        string username = _conn->getUsername();
        string storedPassword = _conn->getStoredPassword();//注册表中的密文
        string clientEncoded = _packet.msg;//用户的密文

        if (username.empty() || storedPassword.empty() || clientEncoded.empty()) {
            SHMS_LOG_DEBUG("Username or storedPassword or clientEncoded is empty in TASK_TYPE_LOGIN_SECTION2");
            return;
        }

        if(!(storedPassword == clientEncoded)){
            //登录失败
            SHMS_LOG_DEBUG("clientEncoded error");
            //告知对端有问题
            _conn->sendPacket(TASK_TYPE_LOGIN_SECTION2_RESP_ERROR,"ERROR");
            return;
        }

        //登陆成功
        string msg = "encoded:" + clientEncoded;
        SHMS_LOG_INFO(msg.c_str());

        //构造TLV，发送给对端(成功)
        _conn->sendPacket(TASK_TYPE_LOGIN_SECTION2_RESP_OK,"OK");
    }
    cout << endl;
}

//若用户名不存在，则在服务端生成salt，发送给client
void UserRegistreSection1::process(){
    cout << "Registre section 1 is processing" << endl;
    //执行用户注册阶段1操作

    if(_packet.type == wd::TASK_TYPE_REGISTER_SECTION1){
        //消息内容为用户名
        string username = _packet.msg; 

        //添加错误处理
        if(username.empty()){
            SHMS_LOG_DEBUG("Username is empty in TASK_TYPE_REGISTER_SECTION1");
            return;
        }

        //获取数据库连接
        auto &db = Configuration::getInstance()->getDBClient();

        string sql = "SELECT salt,encrypt FROM Smart_User WHERE name = '" + username + "'"; 
        auto result = db.readOperationQuery(sql);

        if(!result.empty()){
            //用户名已存在，发送注册失败的消息
            SHMS_LOG_DEBUG("username did exist,Registre falied");
            //告知对端有问题
           _conn->sendPacket(TASK_TYPE_REGISTER_SECTION1_RESP_ERROR,"ERROR");
            return;
        }
        //用户名不存在，则在服务器生成salt，发送给客户端
        char *salt = GenRandomString(8);
        cout << "salt: " << salt << endl;

        string msg= "salt: " + string(salt) + " in TASK_TYPE_REGISTER_SECTION1";
        SHMS_LOG_INFO(msg.c_str());
        
        //保存用户名和盐值
        _conn->setUsername(username);
        _conn->setSalt(salt);

       // //构造TLV，发送给对端（成功）

        _conn->sendPacket(TASK_TYPE_REGISTER_SECTION1_RESP_OK,salt);
    }
    cout << endl;
}
//保存密文，并告诉客户端注册成功返回一个注册成功的消息
void UserRegistreSection2::process(){
    cout << "Registre section 2 is processing" << endl;
    //执行用户注册阶段2操作

    if(_packet.type == wd::TASK_TYPE_REGISTER_SECTION2){
        //消息为密文
        string storedPassword = _packet.msg;
        string username = _conn->getUsername();
        string salt = _conn->getSalt();

        if(username.empty() || salt.empty() || storedPassword.empty()){
            SHMS_LOG_DEBUG("username or salt or storedPassword is empty in TASK_TYPE_REGISTER_SECTION2");
            return;
        }

        //获取数据库的连接
        auto &db = Configuration::getInstance()->getDBClient();

        string sql = "INSERT INTO Smart_User(name,salt,encrypt) VALUES('" + username + "','" + salt + "','" + storedPassword + "');";
        bool flag = db.writeOperationQuery(sql);
        if(!flag){
            //插入失败
            SHMS_LOG_DEBUG("INSERT INTO failed");
            //告知对端有问题
            _conn->sendPacket(TASK_TYPE_REGISTER_SECTION2_RESP_ERROR,"ERROR");
            return;
        }
        //插入成功
        string msg = "encoded: " + storedPassword;
        SHMS_LOG_INFO(msg.c_str());

        _conn->sendPacket(TASK_TYPE_REGISTER_SECTION2_RESP_OK,"OK");
    }
    cout << endl;
}


char* UserRegistreSection1::GenRandomString(int length){
    int flag, i;
    char* string;
    srand((unsigned) time(NULL ));
    if ((string = (char*) malloc(length)) == NULL )  {
            printf("malloc failed!flag:14\n");
                return NULL ;
        }
    for (i = 0; i < length+1; i++)  {
        flag = rand() % 3;
            switch (flag)  {
            case 0: string[i] = 'A' + rand() % 26;break;
            case 1: string[i] = 'a' + rand() % 26; break;
            case 2: string[i] = '0' + rand() % 10; break;
            default: string[i] = 'x';break;
            }
    }
    string[length] = '\0';
    return string;
}

}//end of namespace wd
