#include "SmartHomeMonitorServer.h"
#include "Message.h"
#include "Mylogger.h"
#include "BusinessService.h"

namespace wd{
SmartHomeMonitorServer::SmartHomeMonitorServer(int threadNum,
                                               int taskSize,
                                               unsigned short port,
                                               const string & ip)
: _threadpool(threadNum, taskSize)
, _server(port, ip)
{
    printf("SmartHomeMonitorServer()\n");
    using namespace std::placeholders;
    _server.setAllCallbacks(
                            std::bind(&SmartHomeMonitorServer::onConnection,this,_1),
                            std::bind(&SmartHomeMonitorServer::onMessage,this,_1),
                            std::bind(&SmartHomeMonitorServer::onClose,this,_1));
}

void SmartHomeMonitorServer::start(){
    _threadpool.start();
    _server.start();
}

void SmartHomeMonitorServer::stop(){
    _server.stop();
    _server.stop();
}

void SmartHomeMonitorServer::onConnection(wd::TcpConnectionPtr conn){
    
    string msg = conn->toString() + " has connect.";
    SHMS_LOG_INFO(msg.c_str());
}

void SmartHomeMonitorServer::onMessage(wd::TcpConnectionPtr conn){
    cout << "onMessage..." << endl;
    wd::Packet packet;
    int ret = conn->readPacket(packet);//ret表示收到的字节数
    cout << "read:" << ret << "bytes.\n";
    cout << "packet.type: " << packet.type << endl
         << "packet.length:" << packet.length << endl
         << "pakcet.msg:" << packet.msg << endl;


    //handleTask((wd::TaskType)packet.type)
    switch(packet.type){
        case wd::TASK_TYPE_LOGIN_SECTION1:
             {
                 UserLoginSection1 userLogin1(conn,packet);
                 _threadpool.addTask(std::bind(&UserLoginSection1::process,userLogin1));
             }
             break;
        case wd::TASK_TYPE_LOGIN_SECTION2:
             {
                 UserLoginSection2 userLogin2(conn,packet);
                 _threadpool.addTask(std::bind(&UserLoginSection2::process,userLogin2));
             };
             break;
        case wd::TASK_TYPE_REGISTER_SECTION1:
             {
                 UserRegistreSection1 userRegistre1(conn,packet);
                 _threadpool.addTask(std::bind(&UserRegistreSection1::process,userRegistre1));
             };
             break;
        case wd::TASK_TYPE_REGISTER_SECTION2:
             {
                 UserRegistreSection2 userRegistre2(conn,packet);
                 _threadpool.addTask(std::bind(&UserRegistreSection2::process,userRegistre2));
             };
             break;
    }
}

void SmartHomeMonitorServer::onClose(wd::TcpConnectionPtr conn){
    string msg = conn->toString() + " has close.";
    SHMS_LOG_INFO(msg.c_str());
}
}//end of namespace wd
