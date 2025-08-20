#include <func.h>
#include <shadow.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::cin;

struct TLV
{
    int type;
    int length;
    char data[1024];
};
//枚举类型相比宏的优势，是可以做类型检查
enum TaskType
{
    TASK_TYPE_LOGIN_SECTION1 = 1,
    TASK_TYPE_LOGIN_SECTION1_RESP_OK,
    TASK_TYPE_LOGIN_SECTION1_RESP_ERROR,
    TASK_TYPE_LOGIN_SECTION2,
    TASK_TYPE_LOGIN_SECTION2_RESP_OK,
    TASK_TYPE_LOGIN_SECTION2_RESP_ERROR,
};

int main()
{
    //1. 客户端创建套接字
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(cfd, -1, "socket");
    //2. 连接服务器
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8000);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int ret = connect(cfd, 
                      (const struct sockaddr *)&serveraddr,
                      sizeof(serveraddr));
    ERROR_CHECK(ret, -1, "connect");
    cout << "conn has connected.\n";

    while(1)
    {
        cout << ">> username:";
        string username;
        cin >> username;//用户名
                        
        int id = TASK_TYPE_LOGIN_SECTION1;
        int len = username.length();
        //测试: 发用户名
        send(cfd, &id, 4, 0);
        send(cfd, &len, 4, 0);
        send(cfd, username.c_str(), username.size(), 0);
        cout << "send " << 8 + len << " bytes." << endl;

        TLV tlv = {0};
        recv(cfd, &tlv.type, 4, 0);
        recv(cfd, &tlv.length, 4, 0);
        if(tlv.type == TASK_TYPE_LOGIN_SECTION1_RESP_ERROR ){
            cout << "用户名错误，请重新输入：" << endl;
            continue;
        }
        recv(cfd, tlv.data, tlv.length, 0);
        cout << ">> recv setting from server:\n" << tlv.data << ", len:" << tlv.length <<endl;

        cout << ">> password:";
        string password;
        cin >> password;//用户传过来的密码
                     
        //计算密文
        char *encode = crypt(password.c_str(),tlv.data);

        id = TASK_TYPE_LOGIN_SECTION2;
        len = strlen(encode);
        //测试: 发用户名
        send(cfd, &id, 4, 0);
        send(cfd, &len, 4, 0);
        send(cfd, encode,len, 0);
        cout << "send " << 8 + len << " bytes." << endl;

        tlv = {0};
        recv(cfd, &tlv.type, 4, 0);
        recv(cfd, &tlv.length, 4, 0);
        if(tlv.type == TASK_TYPE_LOGIN_SECTION2_RESP_OK){
            cout << "登陆成功！" << endl;
        }else{
            cout << "登陆失败！" << endl;
        }
    }
    close(cfd);
    return 0;
}

