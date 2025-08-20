#include "SocketIO.h"
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <vector>

using namespace std;

namespace wd{

SocketIO::SocketIO(int fd)
    :_fd(fd){}
int SocketIO::readPacket(Packet &packet) {
    uint32_t netType, netLength; // 明确使用32位无符号整型
    readn((char *)&netType, sizeof(netType));
    readn((char *)&netLength, sizeof(netLength));

    // 网络序转主机序
    packet.type = ntohl(netType);
    packet.length = ntohl(netLength);

    if (packet.length > 0) {
        vector<char> buffer(packet.length + 1); // 改用vector自动管理内存
        readn(buffer.data(), packet.length);
        buffer[packet.length] = '\0'; // 添加终止符
        packet.msg.assign(buffer.data());
    }
    return 8 + packet.length; // 返回总字节数
}

//int SocketIO::readPacket(Packet &packet){
//    //TLV： type(4B)|length(4B)| value
//    //当legnth为0时，表示没有value
//    //type+length 可以认为是packet的header头部
//    //value 可以认为是packet的body消息体
//
//    int type,length;
//    readn((char *)&type,sizeof(type));
//    readn((char *)&length,sizeof(length));
//    cout << "111111111111111" << endl;
//    if(length > 0){
//        char *pbuf = new char[length + 1]();//要确保读取length个字节的数据，不能仅仅只是使用recv
//        cout << "22222222222222" << endl;
//        int ret = readn(pbuf,length);
//        cout << "333333333333333" << endl;
//        packet.type = ntohl(type);// 网络序 --> 主络序
//        cout << "444444444444444444444" << endl;
//        packet.length = ntohl(length);
//        cout << "55555555555555555555" << endl;
//        packet.msg.assign(pbuf,length);//复制length个字节的数据，保存到string中
//        cout << "666666666666666666666" << endl;
//        delete [] pbuf;//切记不要忘了释放
//
//        return ret + 8;//表示整个Packet的大小
//    }//小于0，表示没有value值
//
//    return 8;
//}

//// 发送前统一转网络字节序
//void SocketIO::sendPacket(const Packet& packet) {
//    uint32_t netType = htonl(packet.type); // 字节序转换[3](@ref)
//    uint32_t netLength = htonl(packet.msg.size());
//    
//    sendn(reinterpret_cast<char*>(&netType), sizeof(netType));
//    sendn(reinterpret_cast<char*>(&netLength), sizeof(netLength));
//    sendn(packet.msg.data(), packet.msg.size()); // 直接发原始数据
//}

//因为系统内核缓冲区过小，所以有时候不能一次性读完或者写完，所以通常需要分批次进行读写

//来自readline的调用
//buff是MSG_PEEK拷贝的数据，len是包含'\n'的第一行数据
//读操作最后的目的是让TcpConnetc::receive()中的buff[65536]中存储接受的数据，并返回接收的字节数量

//只有这个函数能改变 TcpConnection类中的 char buff[65536] = {0};buff指针的位置，readline和readPeek都不可以
int SocketIO::readn(char *buff,int len){
    int left = len;//表述数组大小
    char *pbuf = buff;
    //cout << "readn" << endl;
    while(left > 0){//避免一次读不完,分批读
        int ret = recv(_fd,pbuf,left,0);
        if(ret == -1 && errno == EINTR){
            continue;
        }else if(ret == -1){
            perror("recv");            
            return len - left;
        }else if(ret == 0){
            //代表客户端断开连接,返回客户端断开连接前从内存缓冲区接收到的数据个数
            return len - left;//总长度-（总长度-消耗的长度） = 读取的总字节数
        }else{//ret > 0的情况
            left -= ret;//数组还剩多少空间
            pbuf += ret;//将指针向后移动，继续接收信息
        }
    }
    return len - left;//len为0
}

int SocketIO::sendn(const char *buff,int len){
    int left = len;
    const char * p = buff;
    while(left > 0) {
        int ret = ::send(_fd, p, left, 0);
        if(ret == -1 && errno == EINTR)
            continue;
        else if(ret == -1) {
            perror("write");
            return len - left;
        } else {
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

void SocketIO::sendPacket(int type, const std::string& data) {
    uint32_t netType = htonl(type);
    uint32_t netLength = htonl(data.size());

    // 发送头部（8字节）
    sendn(reinterpret_cast<char*>(&netType), sizeof(netType));
    sendn(reinterpret_cast<char*>(&netLength), sizeof(netLength));

    // 发送数据体（实际长度）
    sendn(data.data(), data.size());
}
//maxlen:表示一行最大字节数---数组的大小65536
int SocketIO::readline(char *buff,int maxlen){
    //获取的一行的数据，'\n'是作为边界而出现的

    int left = maxlen - 1;//-1是因为最后要添加'\0'
    char *pbuf = buff;//这里的buff是TcpConnection::receive()中的buff的地址传递
    int ret;
    int total = 0;//表示读取的总字节数
                  //cout << "readline" << endl;
    while(left > 0){//避免一次读不完,分批读
        ret = recvPeek(pbuf,left);//pbuf地址传递
                                  //查找'\n'
        for(int i = 0; i != ret; ++i){//查看从recv读取的字节中有没有'\n'
            if(pbuf[i] == '\n') {   
                int sz = i + 1;//表述包含'\n'的字节数
                               //    cout << "readline inside readn" << endl;
                readn(pbuf,sz);
                total += sz;
                pbuf += sz;
                *pbuf = '\0';
                return total;
            }
        }

        //这一次没有找到'\n',从内核接收缓冲区移走
        //cout << "readline outside readn" << endl;
        readn(pbuf,ret);//读出来的目是为了，执行recvPeek时，再从头开始“拷贝”内存缓冲区中的数据
        left -= ret;//数组还剩多少空间
        pbuf += ret;//将指针向后移动，继续接收信息
        total += ret;
    }
    //没有找到'\n'
    *buff = '\0';//没有找到'\n'，则将整条数据变成一行
    return total;//不包括'\0'

}

int SocketIO::recvPeek(char *buff,int maxlen)const{
    //        cout << "recvPeek" << endl;
    int ret = 0;//读取的字节数
    do{
        //多次接收数据，ret会累加，buf的指针不会变，一直在begain()
        ret = recv(_fd,buff,maxlen,MSG_PEEK);//MSG_PEEK--查看数据，并不从系统缓冲区移走数据
    }while(ret == -1 && errno == EINTR);
    if(ret < 0){
        perror("recv");
    }
    return ret;//读取的字节数
}

}//end of namespace wd
