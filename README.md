# 智能家居监控系统需求文档

# 项目整体架构

## 1、项目开发环境

### **服务器端**

> 操作系统: `Ubuntu22.04`
>
> 编程语言：`C++11`
>
> 编译器：`g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0`
>
> 编辑器：`VIM - Vi IMproved 8.2 (2019 Dec 12, compiled Apr 02 2025 12:39:01)`

### **客户端一**

> 操作系统: `Ubuntu22.04`
>
> 编程语言： `C语言`
>
> 编译器：`g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0`
>
> 编辑器：`VIM - Vi IMproved 8.2 (2019 Dec 12, compiled Apr 02 2025 12:39:01)`

### **客户端二**

>操作系统：`Windows11`
>
>编程语言：`C++`
>
>开发框架：`Qt 5.14.2`
>
>开发工具：`Qt Creator 4.11.1`

### **Qt参考效果图**

![image-20250527002650415](./assets/image-20250527002650415.png)



## 2、服务器目录结构

conf/server.conf	      存放服务器配置文件

include/*.hpp		   存放源码的头文件`*.h` 或 `*.hpp`

src/*.cc				 存放源码的实现文件

log/server.log		   存放项目的日志文件

data/			             存放录制的视频文件

Makefile			      编译项目的Makefile文件



## 3、配置文件

服务器启动时，需要读取配置文件中的信息。

```shell
$ ./SmartHomeMonitorServer ./conf/server.conf
```

配置文件`server.conf`中内容的格式

> ip			127.0.0.1
>
> port 	 	  8000
>
> thread_num     4
>
> task_num	 10000
>
> video_path	./data/
>
> log_file		./log/server.log



## 4、项目架构图1.0

![image-20250525204618420](D:\Wangdao\Qt\assets\image-20250525204618420.png)



## 5、一期类图设计

![image-20250530175338554](./assets/image-20250530175338554.png)





# 一期功能

## 1、配置文件类实现

使用**单例模式**实现**Configuration**类，通过它读取服务器程序的输入信息，之后再启动服务器。



## 2、服务器日志记录

在用户操作的过程中，将用户的所有操作都作为日志记录下来，保存在服务器端，比如：

1）用户注册的操作。（注册成功时）

2）用户登录的操作。（用户登录成功时）

3）用户查看某个摄像头的信息。

服务器的日志记录操作，可以采用之前所学的`Log4cpp（spdlog - 公司推荐使用）`日志系统来完成，比如实现一个**单例模式**的`MyLogger`类。



## 3、服务器框架搭建

服务器框架使用我们之前已经写好的`Reactor + Threadpool` 模型， 在此基础上进行**任务(业务逻辑)的扩展**。

服务器端实现任务解析。我们从需求出发，知道服务器要处理的**任务（业务逻辑）**有以下几种情况：

> - 用户注册
> - 用户登录
> - 保存摄像头信息
> - 获取摄像头的实时流
> - 录制视频
> - 录制视频的开关功能
> - 保存智慧厨房信息

每一种任务的处理方式都是不同的，那我们就需要处理不同的消息，因此我们需要先对**消息格式**进行定义。在此，我们采用`TLV`的消息格式。

![image-20250525223150342](./assets/image-20250525223150342.png)

### TLV格式

TLV（Type-Length-Value，类型 - 长度 - 值）是一种结构化数据编码格式，用于将数据组织成可解析的单元。它通过三个字段（类型、长度、值）的组合来表示信息，广泛应用于网络协议、配置文件、二进制数据传输等场景。

TLV 格式由三个必填字段组成，顺序固定：

- 类型（Type）
  标识数据的类型（如整数、字符串、对象等），通常用 1 字节或多字节 的数值表示（具体长度取决于协议设计）。
  示例：0x01 表示整数，0x02 表示字符串。
- 长度（Length）
  标识值字段（Value）的字节长度，常见编码方式包括：
  定长编码：长度字段固定为 1 字节或 4 字节（如 1 字节表示 0~255，4 字节表示更大范围）。
  变长编码：使用前缀字节表示长度是否超过某个阈值（如 ASN.1 的 BER 编码中，0x80 + 后续字节表示长长度）。
  示例：若值为字符串 "abc"（3 字节），长度字段为 0x03。
- 值（Value）
  存储实际数据，可以是原始字节、嵌套的 TLV 结构或其他复杂类型。
  值的解析依赖于类型字段的定义（如类型为字符串时，按字符集解码；类型为 TLV 集合时，递归解析子 TLV）。

### 示例1 简单整型数据

> Type (1字节) | Length (1字节) | Value (4字节)
>         0x01         |            0x04           | 0x00000064         // 数值100（0x64）

### 示例2 简单字符串

> Type (1字节) | Length (1字节) | Value (N字节) 
>
> ​        0x02         |           0x05           | "hello"                // 字符串长度5，内容为"hello"

### 示例3 嵌套TLV

>Type (1字节) | Length (1字节) |   Value（嵌套TLV） 
>
>​        0x03         |       0x0B         | [    
>
>​									Type=0x01, Length=0x04, Value=0x000001E8  // 整数500                
>
>​									Type=0x02, Length=0x03, Value="abc"              
>
>​								]



### TLV优点

1. 结构灵活，支持嵌套和扩展
2. 二进制格式，空间效率高
3. 自描述性强，字段含义明确

### TLV缺点

1. 解析复杂度较高，需处理长度字段逻辑
2. 可读性差，需依赖协议文档解析
3. 跨平台需处理字节序（大端 / 小端）



### 服务器消息格式

经过以上分析，每一条服务器和客户端之间的消息[**自定义应用层协议**]都应该满足以下格式：

| 消息ID（Type）4B | 消息长度（Length）4B | 消息内容（Value） |
| :--------------: | :------------------: | :---------------: |



## 4、密码验证

为了实现用户注册和登录功能，我们先来了解一下Linux操作系统的用户登录功能。

对于Linux的登陆验证，我想大家可能都知道，是将输入的密码进行相应编码加密后与**/etc/shadow**文件中对应的密码部分进行比较，如果相同则验证通过，如果不同则表明密码错误，但是问题是我们要如何将用户输入的密码加密然后进行比较呢？

为了提高安全性，Linux引入了salt，所谓**salt**即为一个随机数。当用户设置密码时，Linux会随机生成一个salt与用户的密码一起加密，得到一个加密的字符串(salt以明文形式包含在该字符中），存储到密码文件中。crypt函数可以将用户输入的密码和salt一起使用某种算法进行加密，该加密后的字符串就是我们需要的与密码文件中密码对比的加密字符串。

```c
#include <crypt.h>

char *crypt(const char *phrase, const char *setting);
```

`crypt(3) `支持的所有哈希方法都会生成一个由四部分组成的哈希密码短语：前缀（prefix）、选项（options）、盐值（salt）和哈希值（hash）。其中第二个参数`setting`是由**前缀+选项+盐值**组成的。

**前缀**用于控制所使用的哈希方法，它也是传递给` crypt_gensalt(3)` 以选择该方法的对应字符串。
选项、盐值和哈希值的具体内容由所选哈希方法决定。

根据所使用的方法，**前缀**和**选项部分**可能为空。

    前缀id分别为以下几种情况，表示使用不同的哈希方法          
     1   | MD5
     2a  | Blowfish 
     5   | SHA-256 (since glibc 2.7)
     6   | SHA-512 (since glibc 2.7)   
     y   | yescrypt 当前我们Linux采用的加密算法

### yescrypt示例

> $y$j9T$1ZaqJKKvLLwVYOncaUX9q/$gK0jeOlBr1d0KMo2BICoDNYE/23A59EYt3dKql5jz70
>
>   ↑   ↑    ↑                                                        ↑ 
>
> 前缀 选项 盐值                                            哈希值

- 前缀 `$y$` 表示使用 yescrypt 算法。这是目前Linux默认的加密算法

### md5crypt示例

> $1$salt1234$DWd0zR7tJylY/.zWqk1NV0
>
>   ↑      ↑                     ↑
>
> 前缀  盐值            哈希值

- 前缀：$1$
- 盐值：salt1234（**8 字符**）
- 哈希部分：$DWd0zR7tJylY/.zWqk1NV0     （ 由 MD5 ("password" + salt) 计算后编码得到）。

这样我们就可以利用crypt函数将用户输入的字符加密，然后与密码文件中的密码进行对比了。



有一个函数`getspnam`可以根据用户名从`/etc/shadow`中得到密码，函数返回的数据结构为

```c
struct spwd {
	char *sp_namp;     /* Login name */
	char *sp_pwdp;     /* Encrypted password */
	long  sp_lstchg;   /* Date of last change (measured
                          in days since 1970-01-01 00:00:00 +0000 (UTC)) */
	long  sp_min;      /* Min # of days between changes */
	long  sp_max;      /* Max # of days between changes */
	long  sp_warn;     /* # of days before password expires
                          to warn user to change it */
	long  sp_inact;    /* # of days after password expires
                          until account is disabled */
	long  sp_expire;   /* Date when account expires (measured
                          in days since 1970-01-01 00:00:00 +0000 (UTC)) */
    unsigned long sp_flag;  /* Reserved */
};
```

我们现在用自己的用户`lwh`，来查看一下, 这是创建之后从文件`/etc/shadow`中取出来的密码部分

```shell
$y$j9T$1ZaqJKKvLLwVYOncaUX9q/$gK0jeOlBr1d0KMo2BICoDNYE/23A59EYt3dKql5jz70
```

其中 `$y$j9T$1ZaqJKKvLLwVYOncaUX9q/$` 要作为crypt函数的第二个参数setting来传递。

最后写个程序，做下验证

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <shadow.h>
#include <errno.h>

void help(void)
{
    printf("用户密码验证程序\n 第一个参数为用户名!\n");
    exit(-1);
}

void error_quit(char *msg)
{
    perror(msg);
    exit(-2);
}

void get_setting(char *salt,char *passwd)
{
    int i,j;
    //取出salt,i 记录密码字符下标，j记录$出现次数
    for(i = 0,j = 0;passwd[i]&& j!=4;++i)
    {
        if(passwd[i] == '$')
            ++j;
    }
    strncpy(salt,passwd,i);
}

int main(int argc,char *argv[])
{
    struct spwd *sp;
    char *passwd;
    char setting[512] = {0};
    if(argc != 2)
        help();
    passwd = getpass("请输入密码:");
    printf("passwd:%s\n", passwd);

    //读取/etc/shadow文件中对应的用户名的
    //加密密文信息
    if((sp = getspnam(argv[1]))==NULL)
    {
        error_quit("获取用户名和密码");
    }

    get_setting(setting,sp->sp_pwdp);
    setting[strlen(setting) - 1] = '\0';
    printf("setting:%s\n", setting);
    char * encoded = crypt(passwd, setting);
    printf("encrypted: %s\n", encoded);

    if(strcmp(sp->sp_pwdp, encoded)==0)
        printf("验证通过!\n");
    else
        printf("验证失败!\n");
    return 0;
}
```

**注意**：

> 1．`gcc`编译该程序时需要加入库 `-lcrypt`
> 2．程序执行时，需要用`sudo`权限才能执行。



### 网络版密码验证流程

在理解了Linux的密码验证功能以后，就需要将其改造成网络版本的，也就是客户端与服务器的交互过程中，完成登录操作。

为了防止密码泄露，以下是具体的执行流程：

![image-20250525220740925](./assets/image-20250525220740925.png)

从以上执行流程中可以看出，**密码验证**需要分成两个阶段：

```c
enum TaskType {
    TASK_LOGIN_SECTION1 = 1,
    TASK_LOGIN_SECTION1_RESP_OK,
    TASK_LOGIN_SECTION1_RESP_ERROR,
    TASK_LOGIN_SECTION2,
    TASK_LOGIN_SECTION2_RESP_OK,
    TASK_LOGIN_SECTION2_RESP_ERROR,
};
```

**注意**：

1. 客户端直接在Linux操作系统上，采用C语言实现，暂时不需要图形化界面，通过该功能的实现，理解用户登录的流程，为二期功能做准备。
2. 加密时，建议采用`yescrypt`实现加密的哈希值，即采用`ubunt22.04`的默认加密算法。



# 二期功能

## 1、`MySQL`数据读写功能封装

该类在服务器端使用，用来访问`MySQL`数据库。

#### `MySQLClient.hpp`头文件

```c++
#ifndef __MySQLClient_H__
#define __MySQLClient_H__

#include <mysql/mysql.h>

#include <string>
#include <vector>
using std::string;
using std::vector;

namespace wd
{
class MySQLClient
{
public:
    MySQLClient();
    ~MySQLClient();
    
    bool connect(const string & host,
                 unsigned short port,
                 const string & user,
                 const string & passwd,
                 const string & db);
    bool writeOperationQuery(const string & sql);
    vector<vector<string>> readOperationQuery(const string & sql);

    static void dump(const vector<vector<string>> & res);

private:
    void init();

private:
    MYSQL _conn;
};

}//end of namesapce wd
#endif
```

#### `MySQLClient.cc` 实现文件

```c++
#include "MySQLClient.hpp"
#include <iostream>
#include <mysql/mysql.h>


using std::cout;
using std::endl;

namespace wd
{

MySQLClient::MySQLClient()
{
    MYSQL * pconn = mysql_init(&_conn);
    if(!pconn) {
        cout << "mysqlclient init error" << endl;
    }
}

MySQLClient::~MySQLClient()
{
    mysql_close(&_conn);
}


bool MySQLClient::connect(const string & host,
             unsigned short port,
             const string & user,
             const string & passwd,
             const string & db)
{
    MYSQL * pconn = mysql_real_connect(&_conn,
                                       host.c_str(),
                                       user.c_str(),
                                       passwd.c_str(),
                                       db.c_str(),
                                       port,
                                       nullptr,
                                       0);
    if(!pconn) {
        printf("(%d, %s)\n", mysql_errno(&_conn), mysql_error(&_conn));
        return false;
    } else 
        return true;
}

//执行INSERT/UPDATE/DELTE语句
bool MySQLClient::writeOperationQuery(const string & sql)
{
    int ret = mysql_real_query(&_conn, sql.c_str(), sql.size());
    if(ret != 0) {
        printf("(%d, %s)\n", mysql_errno(&_conn), mysql_error(&_conn));
        return false;
    } else {
        printf("Query OK, %ld row(s) affected.\n", mysql_affected_rows(&_conn));
        return true;
    }
}

//执行select语句的查询
vector<vector<string>> MySQLClient::readOperationQuery(const string & sql)
{
    cout << "1111" << endl;
    int ret = mysql_real_query(&_conn, sql.c_str(), sql.size());
    if(ret != 0) {
        printf("(%d, %s)\n", mysql_errno(&_conn), mysql_error(&_conn));
        return vector<vector<string>>();
    }
    cout << "2222" << endl;

    vector<vector<string>> queryRes;;
    MYSQL_RES * res = mysql_store_result(&_conn);
    if(!res) {
        printf("(%d, %s)\n", mysql_errno(&_conn), mysql_error(&_conn));
        return vector<vector<string>>();
    }
    //有结果集的情况
    int rows = mysql_num_rows(res);
    if(rows == 0) {
        return vector<vector<string>>();
    }
    queryRes.reserve(rows + 1);

    int cols = mysql_num_fields(res);
    printf("rows: %d, cols: %d\n", rows, cols);

    MYSQL_FIELD * filed = nullptr;
    vector<string> fileds;
    while((filed = mysql_fetch_field(res)) != nullptr) {
        fileds.push_back(filed->name);
    }
    queryRes.push_back(std::move(fileds));//转移到容器中
    cout << "333" << endl;
                                          
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res)) != nullptr) {
        vector<string> tmp;
        tmp.reserve(cols);
        for(int i = 0; i < cols; ++i) {
            if(row[i] != NULL) {
                tmp.push_back(string(row[i]));
            } else  {
                tmp.push_back(string());
            }
            //cout << "i:" << i << endl;
        }
        //获取一行数据，加入到结果集中
        queryRes.push_back(tmp);
    }
    cout << "444" << endl;

    mysql_free_result(res);
    return queryRes;
}

//对结果集进行打印操作
void MySQLClient::dump(const vector<vector<string>> & res)
{
    cout << "result set has " << res.size() << " row(s)" << endl;
    for(auto & vec : res) {
        for(auto & filed : vec) {
            cout << filed << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

}//end of namespace wd
```

#### `TestMySQL.cpp`测试文件

```c++
#include "MySQLClient.hpp"

#include <cstdlib>
#include <mysql/mysql.h>


int main()
{
    using namespace wd;
    MySQLClient myclient;
    bool ret = myclient.connect("192.168.30.129", 3306, "root", "1234", "embedded1th");
    if(!ret) {
        return EXIT_FAILURE;
    }

    //string sql1 = "INSERT INTO person VALUES(14, 'beiliya', 40, 60, 60, 60)";
    //myclient.writeOperationQuery(sql1);
    string sql2 = "select * from person";
    vector<vector<string>> res = myclient.readOperationQuery(sql2);
    MySQLClient::dump(res);

    return 0;
}
```





## 2、用户注册

### 用户表设计

当**客户端**要访问智能家居监控系统时，需要**先进行用户注册**，注册成功后，才能进行系统登录。

用户注册时，**服务器端**通过关系型数据库`MySQL`**存储用户信息**，包括用户名、盐值、加密密文。

|  字段   |    类型     |               备注说明               |
| :-----: | :---------: | :----------------------------------: |
|   id    |     int     |                 主键                 |
|  name   | varchar(20) |                用户名                |
| setting |    char     | 这里字符串是由【前缀+选项+盐值】组成 |
| encrypt |    char     |               加密密文               |

在做二期的用户注册功能时，**需要使用Qt来实现客户端**了。因为Qt中没有采用`yescrypt`算法生成加密密文的接口了，所以建议直接采用MD5算法来进行加密，即采用`$1$salt1234$DWd0zR7tJylY/.zWqk1NV0`这样的字符串来存储用户的加密密文。用户注册的具体流程如下：

![image-20250525231117465](./assets/image-20250525231117465.png)

### 随机盐值生成

服务器端需要在注册时，给用户生成一个随机的盐值，以下是生成随机字符串的算法，仅供参考：

```c
char* GenRandomString(int length)  {  
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
            case 0:  string[i] = 'A' + rand() % 26;  break;  
            case 1:  string[i] = 'a' + rand() % 26;  break;  
            case 2:  string[i] = '0' + rand() % 10;  break;  
            default:  string[i] = 'x';  break;  
        }
    }
    string[length] = '\0';
    return string;  
} 
```



### Qt中的加密算法

```c++
//CryptString.h头文件
#include <QCryptographicHash>
#include <QString>

class CryptString
{
public:
    CryptString(const QString & data);

    QString generateMD5() const;

private:
    QString _data;
};


//CryptString.cpp实现文件
#include "CryptString.h"
CryptString::CryptString(const QString & data)
    : _data(data)
{}

QString CryptString::generateMD5() const
{
    QByteArray arr = QCryptographicHash::hash(_data.toUtf8(), QCryptographicHash::Md5);
    return arr.toHex();
}
```

**注意**：在Qt中需要自己实现用户注册的界面了，这里大家可以自由发挥。



## 3、用户登录

在一期功能中，我们已经理解了登录的流程。在这里实现时，只需要采取相同的思想即可。需要变化的地方就是在用户登录时，需要进行**数据库查询**了。

- 第一步，查询用户名是否存在。
- 第二部，查询密码是否正确。

具体的登录执行流程如下：

![image-20250525233446355](./assets/image-20250525233446355.png)





# 三期功能

**RTSP协议简介**

`RTSP`（Real Time Streaming Protocol），`RFC2326`，实时流传输协议，是`TCP/IP`协议体系中的一个应用层协议，由哥伦比亚大学、网景和`RealNetworks`公司提交的`IETF RFC`标准。该协议定义了一对多应用程序如何有效地通过IP网络传送多媒体数据。`RTSP`在体系结构上位于`RTP`和`RTCP`之上，它使用`TCP`或`UDP`完成数据传输。`HTTP`与`RTSP`相比，`HTTP`请求由客户机发出，服务器作出响应；使用`RTSP`时，客户机和服务器都可以发出请求，**即`RTSP`可以是双向的**。

`RTSP`是用来**控制声音或影像**的多媒体串流协议，并允许同时多个串流需求控制，传输时所用的网络通讯协定并不在其定义的范围内，服务器端可以自行选择使用`TCP`或`UDP`来传送串流内容，它的语法和运作跟`HTTP 1.1`类似，但并不特别强调时间同步，所以比较能容忍网络延迟。

## 1、使用`VLC`库查看摄像头`RTSP`实时流 

**拉流**： 从摄像头获取视频流信息到本地

**推流**：将本地的视频流推送到远程客户端

`RTSP`测试url:   

- `rtsp://admin:admin@192.168.6.62/live/chn=0` （**可用**）
- `rtsp://admin:admin@192.168.6.69/live/chn=0`(暂不可用)



### libvlc_media_new_location函数

`libvlc_media_new_location` 是 `VLC` 媒体播放器的 `C API（libVLC`）中的一个函数，其主要作用是创建一个基于媒体位置（如 URL 或文件路径）的媒体对象。这个函数是 `libVLC` 库中用于媒体播放的基础函数之一。

#### 函数原型

```c
libvlc_media_t *libvlc_media_new_location(libvlc_instance_t *p_instance, const char *psz_mrl);
```

> 参数：
>
> - `p_instance`：VLC 实例指针，通过libvlc_new函数创建，代表一个 VLC 运行实例。
> - `psz_mrl`：媒体资源定位符（`MRL，Media Resource Locator`），可以是本地文件路径（如`file:///path/to/video.mp4`）或网络 URL（如 `rtsp://localhost:8554/rtsp`）。
>
> 返回值：
>
> - 成功时返回一个新的`libvlc_media_t`对象指针，表示一个媒体资源。
> - 失败时返回NULL。



### 测试用例

使用之前已经写好的`VLCKits`类来完成`rtsp`流的播放

#### VLCKits中的playURL函数

```c++
//VLCKits.cpp文件
bool VLCKits::playURL(const QString &name, void *hwnd)
{
    _pMedia = libvlc_media_new_location(_pInstance, name.toStdString().c_str());
    if(!_pMedia) {
        return false;
    }
    libvlc_media_player_set_media(_pMediaPlayer, _pMedia);

    libvlc_media_player_set_hwnd(_pMediaPlayer, hwnd);
    int ret = libvlc_media_player_play(_pMediaPlayer);
    if(ret < 0) {
        return false;
    } else
        return true;
}
```

#### UI界面布局

<img src="./assets/image-20250527072544366.png" alt="image-20250527072544366" style="zoom:50%;" />

<img src="./assets/image-20250527072622523.png" alt="image-20250527072622523" style="zoom:50%;" />

#### “打开文件”按钮槽函数

```c++
void Widget::on_btnOpen_clicked()
{
    QString url = ui->urlEdit->text();
    qDebug() << "url:" << url << endl;

    _pVLCKits->playURL(url, (void*)ui->videoWidget->winId());
}
```



## 2、使用`FFmpeg`查看摄像头`RTSP/RTMP`实时流

`RTSP`测试`url`:   `rtsp://admin:admin@192.168.106.122/live/chn=0`

`RTMP`测试`url`: `rtmp://192.168.106.122:1935/hlsram/live1`

### **代码实现流程图**

![image-20250529114355347](./assets/image-20250529114355347.png)



### **sws_getContext函数**

`sws_getContext` 是 `FFmpeg` 中用于视频图像转换的核心函数，主要用于创建和配置一个图像转换上下文（`SwsContext`）。这个上下文包含了从源图像（如解码后的视频帧）到目标图像（如显示所需的格式）进行转换所需的所有参数和算法。

#### 函数原型

```c
struct SwsContext *sws_getContext(
    int srcW, int srcH, enum AVPixelFormat srcFormat,
    int dstW, int dstH, enum AVPixelFormat dstFormat,
    int flags, SwsFilter *srcFilter, SwsFilter *dstFilter,
    const double *param
);
```

> 参数：
>
> 1. **源图像参数**：
>    - `srcW`, `srcH`：源图像的宽度和高度（像素）。
>    - `srcFormat`：源图像的像素格式（如 `AV_PIX_FMT_YUV420P`、`AV_PIX_FMT_NV12`）。
> 2. **目标图像参数**：
>    - `dstW`, `dstH`：目标图像的宽度和高度。
>    - `dstFormat`：目标图像的像素格式（如 `AV_PIX_FMT_RGB24`、`AV_PIX_FMT_YUVJ420P`）。
> 3. **缩放算法标志（`flags`）**：
>    - 控制缩放和转换的质量与速度平衡，常用选项：
>      - `SWS_FAST_BILINEAR`：快速双线性插值（速度最快，质量一般）。
>      - `SWS_BILINEAR`：双线性插值（平衡速度和质量，默认选项）。
>      - `SWS_BICUBIC`：双三次插值（质量较高，速度较慢）。
>      - `SWS_LANCZOS`：Lanczos 插值（高质量，适合大幅缩放）。
>      - `SWS_POINT`：最近邻插值（质量最差，仅用于快速下采样）。
> 4. **滤镜参数（可选）**：
>    - `srcFilter`, `dstFilter`：源和目标图像的滤镜（通常为 `NULL`）。
> 5. **算法参数（可选）**：
>    - `param`：用于微调缩放算法的参数数组（通常为 `NULL`）。
>
> 返回值：
>
> - 成功时返回一个指向 `SwsContext` 的指针，该上下文包含图像转换所需的所有配置信息。
> - 失败时返回 `NULL`，可能原因包括不支持的像素格式、内存分配失败等。

#### 核心功能

> 1. **初始化转换引擎**：
>    - 根据源和目标格式，内部初始化优化的转换函数（如 SIMD 加速的 `YUV` 转 `RGB`）。
> 2. **配置缩放算法**：
>    - 根据 `flags` 参数选择合适的插值算法，平衡速度和质量。
> 3. **处理格式差异**：
>    - 处理不同像素格式（如平面 vs 打包）、色彩空间（如 `BT.601` vs `BT.709`）和位深度的转换。

### sws_scale函数

`sws_scale` 是 `FFmpeg` 中用于视频图像转换的核心函数，主要用于实现**图像缩放**、**像素格式转换**（如 `YUV` 转 `RGB`）和**色彩空间转换**等操作。该函数利用之前通过 `sws_getContext` 创建的转换上下文，高效地处理视频帧数据。

#### 函数原型

```c
int sws_scale(struct SwsContext *c,
              const uint8_t *const srcSlice[],
              const int srcStride[],
              int srcSliceY, int srcSliceH,
              uint8_t *const dst[],
              const int dstStride[]);
```

> 参数：
>
> 1. **`SwsContext *c`**
>    - 转换上下文，需提前通过 `sws_getContext()` 初始化，包含源 / 目标格式、缩放算法等配置。
> 2. **`const uint8_t *const srcSlice[]`**
>    - 源图像数据指针数组，每个元素指向一个平面（如 YUV 的 Y、U、V 平面）。
> 3. **`const int srcStride[]`**
>    - 源图像每个平面的行字节数（即一行数据的宽度，可能大于实际图像宽度，用于内存对齐）。
> 4. **`int srcSliceY`**
>    - 源图像的起始行（通常为 `0`）。
> 5. **`int srcSliceH`**
>    - 需要处理的行数（通常为源图像高度 `srcH`）。
> 6. **`uint8_t *const dst[]`**
>    - 目标图像数据指针数组。
> 7. **`const int dstStride[]`**
>    - 目标图像每个平面的行字节数。
>
> 返回值：
>
> - 成功时返回处理的行数（通常等于 `srcSliceH`）。
> - 失败时返回负数错误码（但实际中很少返回错误，更多通过检查上下文创建失败来判断）。

#### 核心功能

> 1. **图像缩放**
>    - 支持任意比例的缩放（如 1920×1080 → 1280×720），使用 `sws_getContext` 中指定的算法（如双线性、双三次插值）。
> 2. **像素格式转换**
>    - 支持 `YUV`、`RGB`、灰度等格式间的相互转换（如 `AV_PIX_FMT_YUV420P` → `AV_PIX_FMT_RGB24`）。
> 3. **色彩空间转换**
>    - 自动处理不同色彩空间的转换（如 `BT.601` ↔ `BT.709`）。
> 4. **平面与打包格式转换**
>    - 例如将平面存储的 `YUV420P` 转换为打包存储的 `NV12`。

#### 使用流程

> 1. 创建转换上下文
> 2. 准备源和目标数据缓冲区
> 3. 执行转换
> 4. 释放资源



### **代码实战**

#### `UI`界面线程

`MainWidget.h`头文件

```c++
#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include "FFmpegKits.h"

#include <QWidget>
#include <QImage>

#include <memory>

using std::unique_ptr;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

enum PLAYER_STATE
{
    PLAYER_IDLE = 0,
    PLAYER_PLAYING,
    PLAYER_PAUSE,
    PLAYER_STOP
};

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void paintEvent(QPaintEvent *event) override;

private slots:
    void getOneFrame(QImage image);

    void on_pauseButton_clicked();

    void on_playButton_clicked();

    void on_flipButton_clicked();

private:
    Ui::MainWidget *ui;
    unique_ptr<FFmpegKits> _ffmpegKits;
    QImage _image;
    PLAYER_STATE _kPlayState;
    bool _hFlip;//水平翻转
    bool _vFlip;//垂直翻转
};
#endif // MAINWIDGET_H
```



`MainWidget.cpp`实现文件

```c++
#include "MainWidget.h"
#include "ui_MainWidget.h"

#include <QPainter>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , _ffmpegKits(new FFmpegKits)
    , _kPlayState(PLAYER_IDLE)
    , _hFlip(false)
    , _vFlip(false)
{
    ui->setupUi(this);

    connect(_ffmpegKits.get(), &FFmpegKits::sigGetOneFrame, this, &MainWidget::getOneFrame);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::getOneFrame(QImage image)
{
    ui->imageLabel->clear();
    if(_kPlayState == PLAYER_PAUSE)
    {
       return;
    }

    _image = image;
    update(); //调用update将执行paintEvent函数
}

void MainWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int showWidth = this->width() - 100;
    int showHeight = this->height() - 50;

    painter.setBrush(Qt::white);
    painter.drawRect(0, 0, this->width(), this->height()); //先画成白色

    if (_image.size().width() <= 0)
    {
        return;
    }

    //将图像按比例缩放
    QImage img = _image.scaled(QSize(showWidth, showHeight), Qt::KeepAspectRatio);
    img = img.mirrored(_hFlip, _vFlip);

    int x = this->width() - img.width();
    int y = this->height() - img.height();
    x /= 2;
    y /= 2;
    qDebug()<< "x:" << x << ", y:" << y << endl;
    painter.drawImage(QPoint(x, 100), img); //画出图像
}


void MainWidget::on_pauseButton_clicked()
{}

void MainWidget::on_playButton_clicked()
{
    if (_kPlayState == PLAYER_IDLE) //第一次按下为启动，后续则为继续
    {
        ui->addrText->setEnabled(false);
        //QString url = ui->addrText->text();
        QString url = QString::fromUtf8("rtsp://admin:admin@192.168.106.122/live/chn=0");
        qDebug() << "url:" << url << endl;

        _ffmpegKits->startPlay(url);
        ui->addrText->setText("rtsp网络连接中...");
        _kPlayState = PLAYER_PLAYING;
    }
    else
    {
        _kPlayState = PLAYER_PAUSE;
    }
}

void MainWidget::on_flipButton_clicked()
{
    _hFlip = !_hFlip;
}
```

#### 解码线程

`FFmpegKits.h`头文件

```c++
#ifndef FFMPEGKITS_H
#define FFMPEGKITS_H

#include <QThread>
#include <QImage>
//继承自QThread
class FFmpegKits : public QThread
{
    Q_OBJECT
public:
    explicit FFmpegKits(QObject *parent = nullptr);

    void startPlay(QString url);

protected:
    void run() override;

signals:
    void sigGetOneFrame(QImage image);

private:
    QString _url;
};

#endif // FFMPEGKITS_H
```



`FFmpegKits.cpp`实现文件

```c++
#include "FFmpegKits.h"
#include <QDebug>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}

FFmpegKits::FFmpegKits(QObject *parent)
    : QThread(parent)
{
}

void FFmpegKits::startPlay(QString url)
{
    _url = url;
    this->start();//开始运行耗时线程
}

void FFmpegKits::run()
{
    qDebug() << "FFmpegKits::run()..." << endl;
    AVFormatContext *pFormatCtx = nullptr; //音视频封装格式上下文结构体
    AVCodecContext  *pCodecCtx = nullptr;  //音视频编码器上下文结构体
    AVCodec *pCodec = nullptr; //音视频编码器结构体
    AVFrame *pFrame = nullptr; //存储一帧解码后像素数据
    AVFrame *pFrameRGB = nullptr;
    AVPacket *pPacket = nullptr; //存储一帧压缩编码数据
    uint8_t *pOutBuffer = nullptr;
    static struct SwsContext *pImgConvertCtx = nullptr;

    avformat_network_init();   //初始化FFmpeg网络模块

    //Allocate an AVFormatContext.
    pFormatCtx = avformat_alloc_context();

    //AVDictionary
    AVDictionary *avdic=nullptr;
    char option_key[] = "rtsp_transport";
    char option_value[] = "udp";
    av_dict_set(&avdic, option_key, option_value, 0);

    char option_key2[] = "max_delay";
    char option_value2[] = "100";
    av_dict_set(&avdic, option_key2, option_value2, 0);

    if (avformat_open_input(&pFormatCtx, _url.toStdString().data(), nullptr, &avdic) != 0)
    {
        printf("can't open the file. \n");
        return;
    }

    if (avformat_find_stream_info(pFormatCtx, nullptr) < 0)
    {
        printf("Could't find stream infomation.\n");
        return;
    }

    //查找视频中包含的流信息，音频流先不处理
    int videoStreamIdx = -1;
    qDebug("apFormatCtx->nb_streams:%d", pFormatCtx->nb_streams);
    videoStreamIdx = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if(videoStreamIdx < 0) {
        qDebug() << "av_find_best_stream error:" << av_get_media_type_string(AVMEDIA_TYPE_VIDEO);
        return ;
    }
    qDebug("video stream idx: %d\n", videoStreamIdx);
    //查找解码器
    qDebug("avcodec_find_decoder...\n");
    qDebug() << "AVCodecID:" << pFormatCtx->streams[videoStreamIdx]->codecpar->codec_id << endl;
    pCodec = avcodec_find_decoder(pFormatCtx->streams[videoStreamIdx]->codecpar->codec_id);

    if (pCodec == nullptr)
    {
        qDebug("Codec not found.\n");
        return;
    }

    //开辟解码器空间
    pCodecCtx = avcodec_alloc_context3(pCodec);
    //拷贝解码器参数
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStreamIdx]->codecpar);
    qDebug("pCodecCtx->codec_id: %d\n", pCodecCtx->codec_id);
    //初始化解码器参数
    pCodecCtx->bit_rate = 0;   //初始化为0
    pCodecCtx->time_base.num = 1;  //下面两行：一秒钟25帧
    pCodecCtx->time_base.den = 25;
    pCodecCtx->frame_number = 1;  //每包一个视频帧

    //打开解码器
    if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0)
    {
        printf("Could not open codec.\n");
        return;
    }

    //将解码后的YUV数据转换成RGB32
    //创建转换上下文
    pImgConvertCtx = sws_getContext(
        pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
        pCodecCtx->width, pCodecCtx->height,AV_PIX_FMT_RGB32,
        SWS_BICUBIC, nullptr, nullptr, nullptr);

    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB32, pCodecCtx->width,pCodecCtx->height);

    //准备源和目标数据缓冲区
    pFrame     = av_frame_alloc();//开辟空间
    pFrameRGB  = av_frame_alloc();//开辟空间
    pOutBuffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, pOutBuffer, 
                   AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height);

    pPacket = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
    int y_size = pCodecCtx->width * pCodecCtx->height;
    av_new_packet(pPacket, y_size); //分配packet的数据

    while (1) {
        if (av_read_frame(pFormatCtx, pPacket) < 0) {
            break; //这里认为视频读取完了
        }

        if (pPacket->stream_index == videoStreamIdx) {
            int got_picture;
            //执行解码
            int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,pPacket);
            if (ret < 0)
            {
                printf("decode error.\n");
                return;
            }

            if (got_picture)
            {
                //执行转换
                sws_scale(pImgConvertCtx, (uint8_t const * const *) pFrame->data, pFrame->linesize,
                          0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);

                //把这个RGB数据 用QImage加载
                QImage tmpImg((uchar *)pOutBuffer, 
                              pCodecCtx->width, pCodecCtx->height, QImage::Format_RGB32);
                QImage image = tmpImg.copy(); //把图像复制一份 传递给界面显示
                emit sigGetOneFrame(image);  //发送信号
            }
        }
        av_free_packet(pPacket);
    }

    av_free(pOutBuffer);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}
```

#### 程序效果图

![image-20250529143203300](./assets/image-20250529143203300.png)

完成以上测试以后，咱们可以开始给咱们的智能家居监控系统的**Qt前端UI界面进行布局设置**了。



## 3、前端`UI`布局

这里大家可以自由发挥，以下界面可以作为参考。

![image-20250527002650415](./assets/image-20250527002650415.png)



## 4、迅思维公司

迅思维官网： https://www.xstrive.com/

### 公司介绍

武汉市迅思维科技有限公司成立于2011年，座落在技术氛围浓厚的武汉大学科技园内。公司定位于专业的音视频软件产品开发商，是一家以多媒体软件设计，研发，实施，测试为主营方向的创新型高科技团队。公司致力于向全球各地机构，企业提供整体软硬件解决方案，并提供产品策划、运维、定制开发等全产业链技术服务。迅思维科技成立于2011年，公司管理层80%具有本科以上学历，公司90%人员为软件开发工程师，核心技术人员具有10年以上企业、政府信息化大型项目开发经验。

迅思维科技已搭建完善的网络环境，能够实现异地多项目的在线协同开发，完全满足软件开发服务的需要，在降低开发成本的同时，极大的提高开发效率。公司凭借高素质的技术团队、严格规范的业务流程、细致完善的服务体系，为客户提供高质、高效的软件服务。

完善、及时、良好的服务是公司的发展目标。公司主要研发方向为：rtmp推流器，视频直播技术，视频分析，传统监控直播化，流媒体视频云存储等领域。公司硬件产品主要基于华为海思芯片，形成了以Hi3516、Hi3518、Hi3519、Hi3520、Hi3536、Hi3559等海思全系列芯片，从低端到高端的硬件开发体系，可以满足不同客户的多样化定制需求。 公司自主研发的RTSP转RTMP转码推流器，首创嵌入式推流器，仅火柴盒大小。支持兼容目前市面所有设备的升级改造，包括传统同轴电缆摄像头、IPC网络摄像头、SDI高清摄像头、各种DVR/NVR等设备。仅需简单改造即可将传统视频监控升级为支持`RTMP/HLS`码流的实时视频直播的云监控。

### 产品介绍

枪机和球机是监控系统中常见的两种摄像头类型，主要区别在于外形结构、功能特性和应用场景。以下是具体解析：

#### **枪机**

又名**枪式摄像机**

> - 外形：呈长方体或圆柱形，类似 “枪” 的形状，故名 “枪机”。
> - 结构：通常由镜头、机身和支架组成，镜头可根据需求更换（如定焦、变焦镜头）。
> - 安装方式：需配合支架固定安装，视角固定，部分型号支持红外夜视。

<img src="./assets/image-20250529153223663.png" alt="image-20250529153223663" style="zoom:50%;" />

####  核心特点

> - **固定视角**：镜头方向固定，监控范围单一，需通过多台设备覆盖不同角度。
> - **灵活性低但画质稳定**：适合固定场景（如出入口、走廊），可搭配高清镜头实现高分辨率监控（如 4K）。
> - **成本较低**：结构简单，无旋转部件，适合预算有限或只需固定监控的场景。

#### 典型应用场景

> - 室内外固定点位监控（如店铺门口、楼道、围墙）。
> - 需要长焦距远摄的场景（如搭配长焦镜头监控远距离目标）。
> - 对隐蔽性要求不高，但需高清画质的场景。

#### **球机**

又名**球形摄像机 / 球型云台摄像机**

> - **外形**：呈球形或半球形，内部集成云台和镜头，外壳多为高强度材质（如金属或工程塑料）。
> - **结构**：内置电机驱动的云台，支持水平 360 度旋转、垂直俯仰（如 - 90° 至 + 90°），镜头支持自动变焦（Zoom）。

<img src="./assets/image-20250529153623395.png" alt="image-20250529153623395" style="zoom:50%;" />

#### **核心特点**

> - **全向监控**：通过云台控制实现无死角覆盖，可预设多个监控点位（“预置位”），自动巡航切换。
> - **智能功能丰富**：支持自动跟踪（如人形追踪、车辆追踪）、移动侦测、红外夜视（部分型号支持激光夜视）、透雾模式等。
> - **高灵活性与集成度**：一台球机可替代多台枪机的监控范围，适合需要动态监控的场景。

#### **典型应用场景**

> - **全向监控**：通过云台控制实现无死角覆盖，可预设多个监控点位（“预置位”），自动巡航切换。
> - **智能功能丰富**：支持自动跟踪（如人形追踪、车辆追踪）、移动侦测、红外夜视（部分型号支持激光夜视）、透雾模式等。
> - **高灵活性与集成度**：一台球机可替代多台枪机的监控范围，适合需要动态监控的场景。



#### **测试设备`IP`地址**

1、枪机`IP`: **192.168.106.69**

2、球机`IP`: **192.168.106.122**

### 后台管理页面访问

可以直接通过摄像头的IP地址进行访问，值需要在chrome浏览器中输入IP地址即可，比如：

http://192.168.106.122

![image-20250529154846874](./assets/image-20250529154846874.png)

输入用户名和密码登录后，就可以看到后台管理页面了，在这里可以查看摄像头的数据，不了解时千万别进行修改哈。

![image-20250529155105297](./assets/image-20250529155105297.png)



### 接口说明

迅思维公司的产品可以直接通过**HTTP协议**进行二次开发，完成对于摄像头等产品的操作和控制。

#### 文档地址链接： 

http://showdoc.xstrive.com/web/#/38/258

接口文档提供了很多接口，可以自行参考和测试，以加入到我们自己的项目中。

![image-20250529160739362](./assets/image-20250529160739362.png)



#### URL固定携带参数

一个可行的获取录像列表的HTTP请求URL为：

http://192.168.106.122/xsw/api/record/list?beginTime=1673193600000&endTime=1673279999999&channel=0&t=1673255651&token=80c192912ea8c97523876b47cf2658a1

以上HTTP的请求报文的URL中必须固定携带以下两个参数：**token**和**t**

| 参数名 | 描述       | 类型   | 必传 | 附加说明     |
| :----- | :--------- | :----- | :--- | :----------- |
| token  | 签名串     | String | Y    |              |
| t      | 当前时间戳 | Number | Y    | 单位：**秒** |



#### token生成规则

> 1. 签名使用**md5**加密生成token参数进行传递校验
> 2. 生成md5值时，参数中必须包括秘钥 secret ，默认的密钥为 f6fdffe48c908deb0f4c3bd36c032e72
> 3. 待加密串按Url**参数词典序**进行排序**通过&符号进行拼接**。**如果参数通过body传递，body里面的参数不进行字符串拼接**。
>    参数词典序排序JS代码示例：keys = [key1,key2,…,secret,t]; keys = keys.sort();

**注意**：密钥也可以通过后台管理网页进行设置。（我们开发时建议不修改）

![visitFile](./assets/681c0da5cc2e1.png)



#### token生成示例

1. 生成md5值前，**所有参数**必须按**词典序排序**后，拼接成的字符串 （除token外的所有参数）如下（已排好序）：

> **beginTime**=1673193600000&**channel**=0&**endTime**=1673279999999&**secret**=f6fdffe48c908deb0f4c3bd36c032e72&**t**=1673255651

2. 对上述字符串生成md5值

> 使用MD5哈希算法得到的token值为 80c192912ea8c97523876b47cf2658a1

3. 最终拼接出一个http请求的url

> 最终的HTTP请求Url为: http://192.168.106.122/xsw/api/record/list?beginTime=1673193600000&endTime=1673279999999&channel=0&t=1673255651&token=80c192912ea8c97523876b47cf2658a1



#### 测试代码

`CryptString.h`头文件

```c++
#ifndef CRYPTSTRING_H
#define CRYPTSTRING_H

#include <QString>
#include <map>
#include <string>

using std::map;
using std::string;

class CryptString
{
public:
    CryptString(const QString & data);

    QString generateMD5() const;
private:
    QString _data;
};

class KVQuery
{
public:
    KVQuery();
    void add(const string & key, const string & value);
    void erase(const string & key);
    void clear();

    string toString() const;
    string toCrpytString();
private:
    map<string, string> _map;
};

#endif // CRYPTSTRING_H
```

`CryptString.cpp`实现文件

```c++
#include "CryptString.h"
#include <QCryptographicHash>

#include <QByteArray>
#include <QDateTime>
#include <QString>

CryptString::CryptString(const QString & data)
    : _data(data)
{}

QString CryptString::generateMD5() const
{
    QByteArray arr = QCryptographicHash::hash(_data.toUtf8(), QCryptographicHash::Md5);
    return arr.toHex();
}

KVQuery::KVQuery()
{
    //因为每个请求都需要通过密钥secret和当前时间t来生成token
    //在KVQuery的构造函数中直接加入这两个字段
    _map.insert(std::make_pair("secret", "f6fdffe48c908deb0f4c3bd36c032e72"));
    QDateTime currentDateTime = QDateTime::currentDateTime();
    string secstr = std::to_string(currentDateTime.toSecsSinceEpoch());
    _map.insert(std::make_pair("t", secstr));
}

void KVQuery::add(const std::string &key, const std::string &value)
{
    _map.insert(std::make_pair(key, value));
}

void KVQuery::erase(const std::string &key)
{
    _map.erase(key);
}

void KVQuery::clear()
{
    _map.clear();
}

string KVQuery::toString() const
{   //转换为url中查询词的格式
    string tmp;
    for(auto & elem : _map) {
        tmp.append(elem.first).append("=").append(elem.second).append("&");
    }
    tmp.pop_back();
    return tmp;
}

string KVQuery::toCrpytString()
{
    CryptString cryptStr(QString::fromStdString(toString()));
    QString token = cryptStr.generateMD5();
    this->add("token", token.toStdString());
    //正式生成的url中是不携带secret字段的，因此需要删除
    this->erase("secret");//去掉secret
    return toString();
}
```

**测试用例**

```c++
void genRecordListRequestTest()
{
    KVQuery kvs;
    kvs.add("start_time", "1673193600000");
    kvs.add("end_time", "1673279999999");
    kvs.add("channel", "0");

    QString tmp = QString::fromStdString(kvs.toString());
    qDebug() << "before crypt tmp:" << tmp << endl;
    // ------------------------------------------------
    CryptString cryptStr(tmp);
    QString token = cryptStr.generateMD5();
    qDebug() << "token:" << token << endl;//生成token
    kvs.add("token", token.toStdString());//加入参数
    kvs.erase("secret");//去掉secret

    //重新生成query
    tmp = QString::fromStdString(kvs.toString());
    qDebug() << "query:" << tmp << endl;
    // ------------------------------------------------
    QUrl url("http://192.168.106.122/xsw/api/reord/list");
    url.setQuery(tmp);
    qDebug() << "url:" << url.toString() << endl;
}
```



#### 请求头固定参数

1. 默认请求 content-type 设置

> header[“content-type”] = “application/x-www-form-urlencoded”

2. 带body参数的如无特殊备注 content-type 为

>  header[“content-type”] = “application/json; charset=utf-8”

3. 上传文件时 content-type 为
   
> header[“content-type”] = “multipart/form-data”



### 云台控制（必选功能）

接口说明链接：http://showdoc.xstrive.com/web/#/38/854

在Qt实现的`UI`界面上，通过点击8个方向，**实现对球机的控制**。以下为`UI`界面示意图。

![image-20250529152316509](./assets/image-20250529152316509.png)



### 神器`wireshark`

**抓包分析**

![image-20250604135748112](./assets/image-20250604135748112.png)



![image-20250604135817209](./assets/image-20250604135817209.png)

![image-20250604135834241](./assets/image-20250604135834241.png)

模仿之，亦可以在Qt客户端上移植成功。



# 四期功能

## 1、服务器转发`HTTP`请求

当Qt客户端发送**`Http`请求**时，交给服务器，服务器**转发**给**监控摄像头**进行相应处理。因此服务器需要解析HTTP报文，并打包HTTP报文，常见的HTTP开发框架有多种，用的比较多的是就是`libcurl`.

#### 流程图

 



### 1.1 `HTTP`框架`libcurl`

**libcurl** 是一个开源的跨平台网络传输库，支持多种网络协议（如 HTTP、HTTPS、FTP、SMTP 等），用于在程序中实现客户端网络通信。它由瑞典软件工程师 **Daniel Stenberg** 开发，最初发布于 1998 年，目前由 **curl 项目**维护，广泛应用于操作系统、嵌入式设备、桌面软件和移动应用中。

libcurl 是一个免费且易于使用的客户端 URL 传输库，支持 DICT、**FILE**、FTP、FTPS、GOPHER、GOPHERS、**HTTP**、**HTTPS**、IMAP、IMAPS、LDAP、LDAPS、MQTT、POP3、POP3S、**RTMP**、RTMPS、**RTSP**、SCP、SFTP、SMB、SMBS、SMTP、SMTPS、TELNET、TFTP、WS 和 WSS。

libcurl 支持 SSLcertificates、HTTP POST、HTTP PUT、FTP 上传、基于 HTTP 表单的上传、代理、HTTP/2、HTTP/3、cookie、用户+密码身份验证（Basic、Digest、NTLM、Negotiate、Kerberos）、文件传输恢复、http 代理隧道等！



#### **核心特性**

1. **支持丰富的协议**
   - 主流协议：`HTTP/HTTPS、FTP、SFTP、SMTP、POP3、IMAP` 等。
   - 其他协议：`TELNET、LDAP、RTSP、FILE`（本地文件传输）等。
2. **跨平台兼容性**
   - 支持 `Windows、Linux、macOS、Android、iOS` 等主流操作系统。
   - 可在嵌入式系统（如 `Arduino、Raspberry Pi`）中运行。
3. **灵活的接口**
   - 提供 C 语言接口（`libcurl` 库），也可通过封装用于 `C++、Python、Java` 等语言。
   - 支持同步和异步（非阻塞）请求模式。
4. **强大的功能**
   - 支持 `SSL/TLS` 加密（`HTTPS`）、证书验证、`Cookie` 管理。
   - 支持请求 / 响应的自定义头部、`POST` 数据提交、文件上传下载。
   - 支持断点续传、代理服务器、`HTTP` 压缩（`gzip/deflate`）等。
5. **高性能与低资源占用**
   - 采用事件驱动机制（如 `libevent` 集成），适合高并发场景。
   - 内存管理高效，适合资源受限的环境。



#### **主要组件与概念**

1. **CURL 句柄（CURL）**
   - 所有操作的核心对象，通过 `curl_easy_init()` 创建，用于配置请求参数和执行请求。
2. **选项（Options）**
   - 通过`curl_easy_setopt()`函数设置请求参数，例如：
     - `CURLOPT_URL`：目标 URL。
     - `CURLOPT_POSTFIELDS`：POST 请求的数据。
     - `CURLOPT_SSL_VERIFYPEER`：是否验证服务器证书（`HTTPS`场景）。
     - `CURLOPT_WRITEFUNCTION`：自定义响应数据处理函数。
3. **回调函数**
   - 用于处理请求过程中的数据，例如：
     - `write_callback`：接收响应体数据。
     - `read_callback`：发送请求体数据（如文件上传）。
     - `header_callback`：处理响应头部。
4. **多线程与异步支持**
   - 通过 `libcurl` 的多接口（如 `curl_multi_init()`）实现并发请求，或结合异步框架（如 `libuv`）实现非阻塞操作。

#### **典型使用场景**

1. **HTTP/HTTPS 请求**

   - 发送 `GET/POST` 请求、处理 `JSON/XML` 数据、模拟浏览器行为。
   - 示例代码（C 语言发送 GET 请求）：

   ```c
   #include <curl/curl.h>  
   #include <stdio.h>  
   
   size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {  
       // 处理响应数据（如打印到控制台）  
       fwrite(contents, size, nmemb, (FILE*)userp);  
       return size * nmemb;  
   }  
   
   int main() {  
       CURL *curl = curl_easy_init();  
       FILE *fp = fopen("response.txt", "wb");  
   
       if (curl && fp) {  
           curl_easy_setopt(curl, CURLOPT_URL, "https://api.example.com/data");  
           curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);  
           curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);  
   
           // 执行请求  
           CURLcode res = curl_easy_perform(curl);  
           if (res != CURLE_OK) {  
               fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));  
           }  
   
           curl_easy_cleanup(curl);  
           fclose(fp);
       }  
       return 0;  
   }
   ```

2. **文件传输（`FTP/SFTP`）**

   - 上传 / 下载文件，支持断点续传（`CURLOPT_RESUME_FROM`）。

3. **客户端认证**

   - 支持 `Basic/Digest` 认证、`SSL` 客户端证书（`CURLOPT_SSLCERT`）。

4. **爬虫与数据抓取**

   - 通过设置请求头、`Cookie` 模拟浏览器行为，获取网页内容。

#### **优势与局限**

| **优势**                                                   | **局限**                                 |
| ---------------------------------------------------------- | ---------------------------------------- |
| 跨平台性强，接口稳定                                       | 入门需学习较多选项和回调机制             |
| 社区活跃，文档丰富（[官方文档](https://curl.se/libcurl/)） | 异步编程需结合其他库（如 `libevent`）    |
| 支持几乎所有网络协议                                       | 处理复杂协议（如 `WebSocket`）需额外封装 |
| 资源占用低，适合嵌入式系统                                 | 部分高级功能（如 HTTP/2）需编译时启用    |



#### 安装方式

```shell
$ sudo apt install libcurl4-openssl-dev
```

检查是否安装完毕

```shell
$ curl --version
$ ls /usr/include/curl
```



### 1.2 `JSON`格式解析

迅思维的官方接口文档中，有进行说明，摄像头返回的`Response`中有`JSON`数据，**服务器需要对其进行解析**，提取出其中的字段，此时就一定需要用到`JSON`格式解析的库，这里推荐使用`cJSON`开源库或`nlohmann/json`开源库。

#### `cJSON`安装方式

```c
$ sudo apt install libcjson-dev
```

查看安装路径

```shell
$ ls /usr/include/cjson/
```

#### 测试用例

```c
#include <stdio.h>
#include <cjson/cJSON.h>

int main() {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "Jackie");
    cJSON_AddNumberToObject(root, "age", 40);
    
    char *json_str = cJSON_Print(root);
    printf("%s\n", json_str);
    
    cJSON_free(json_str);
    cJSON_Delete(root);
    
    return 0;
}
```



#### `nlohmann/json`安装方式

下载路径： https://github.com/nlohmann/json.git ，王道服务器上已经上传了，可以直接获取。

![image-20250604135607997](./assets/image-20250604135607997.png)

安装方式：

![image-20250604134436785](./assets/image-20250604134436785.png)

![image-20250604135028633](./assets/image-20250604135028633.png)

#### 测试用例

```c++
#include <nlohmann/json.hpp>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

using nlohmann::json;

int main()
{
    json arr;
    json elem;
    elem["title"] = "404, not found";
    elem["summary"] = "i cannot find what you want";
    elem["url"] = "";
    arr.push_back(elem);

    json root;
    root["files"] = arr;
    string jstr = root.dump(2);
    cout << jstr << endl;
    return 0;
}
```



#### Qt客户端解析`JSON`

在 Qt 里解析 JSON 字符串，你可以使用 QtCore 模块提供的 JSON 类。主要涉及以下类和接口：

1. QJsonDocument
   - 该类能够从 JSON 文本创建文档，也能生成 JSON 文本。

- 相关接口有：
  - `QJsonDocument::fromJson(const QByteArray &json, QJsonParseError *error = nullptr)`：从 UTF - 8 编码的 JSON 文本创建文档。
  - `QJsonDocument::toJson(JsonFormat format = Indented)`：将文档转换为 JSON 文本。
  - `QJsonDocument::isNull()`：判断文档是否为空。
  - `QJsonDocument::isObject()`和`QJsonDocument::isArray()`：检查文档的根类型。
  - `QJsonDocument::object()`和`QJsonDocument::array()`：获取根对象或数组。

2. QJsonObject
   - 它表示 JSON 对象，也就是键值对的集合。

- 提供的接口包括：
  - `QJsonObject::contains(const QString &key)`：检查对象是否包含某个键。
  - `QJsonObject::value(const QString &key)`：获取指定键的值。
  - `QJsonObject::insert(const QString &key, const QJsonValue &value)`：插入或更新键值对。
  - `QJsonObject::keys()`：获取所有键的列表。
  - `QJsonObject::size()`：返回对象中键值对的数量。

3. QJsonArray
   - 用于表示 JSON 数组。

- 接口有：
  - `QJsonArray::append(const QJsonValue &value)`和`QJsonArray::insert(int i, const QJsonValue &value)`：添加元素。
  - `QJsonArray::at(int i)`和`operator[]`：访问元素。
  - `QJsonArray::size()`：返回数组的大小。

4. QJsonValue
   - 可表示 JSON 中的值，支持多种数据类型，像 null、bool、double、string、array 和 object。
   - 接口如下：
     - `QJsonValue::isNull()`、`QJsonValue::isBool()`等：检查值的类型。
     - `QJsonValue::toBool()`、`QJsonValue::toInt()`等：将值转换为特定类型。
     - `QJsonValue::isArray()`和`QJsonValue::isObject()`：检查值是否为数组或对象。
     - `QJsonValue::toArray()`和`QJsonValue::toObject()`：获取数组或对象。

5. QJsonParseError
   - 用于在解析 JSON 时报告错误。

- 相关接口为：
  - `QJsonParseError::error`：获取错误类型。
  - `QJsonParseError::errorString()`：获取错误的文本描述。

#### 解析 JSON 字符串的步骤

1. 借助`QJsonDocument::fromJson()`将 JSON 字符串转换为`QJsonDocument`。
2. 查看解析过程中是否有错误发生。
3. 判断根元素是对象（`QJsonObject`）还是数组（`QJsonArray`）。
4. 对 JSON 数据进行遍历和访问。

#### 示例代码

```c++
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 示例JSON字符串
    QString jsonString = R"({
        "name": "John Doe",
        "age": 30,
        "isStudent": false,
        "hobbies": ["reading", "swimming", "coding"],
        "address": {
            "street": "123 Main St",
            "city": "Anytown",
            "state": "CA"
        }
    })";

    // 将QString转换为QByteArray
    QByteArray jsonData = jsonString.toUtf8();

    // 解析JSON
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    // 检查解析是否成功
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON解析错误:" << parseError.errorString();
        return 1;
    }

    // 检查根是否为对象
    if (jsonDoc.isObject()) {
        QJsonObject rootObj = jsonDoc.object();

        // 访问基本类型字段
        QString name = rootObj["name"].toString();
        int age = rootObj["age"].toInt();
        bool isStudent = rootObj["isStudent"].toBool();

        qDebug() << "Name:" << name;
        qDebug() << "Age:" << age;
        qDebug() << "Is Student:" << isStudent;

        // 访问数组
        QJsonArray hobbiesArray = rootObj["hobbies"].toArray();
        qDebug() << "Hobbies:";
        for (const auto& hobbyValue : hobbiesArray) {
            qDebug() << "-" << hobbyValue.toString();
        }

        // 访问嵌套对象
        QJsonObject addressObj = rootObj["address"].toObject();
        QString street = addressObj["street"].toString();
        QString city = addressObj["city"].toString();
        QString state = addressObj["state"].toString();

        qDebug() << "Address:";
        qDebug() << "- Street:" << street;
        qDebug() << "- City:" << city;
        qDebug() << "- State:" << state;
    } else {
        qDebug() << "JSON根不是对象";
    }

    return a.exec();
}
```



## 2、服务器转发`RTSP/RTMP`实时流

当Qt客户端获取实时`RTSP/RTMP`流时，由服务器转发给Qt客户端。

### 2.1 服务器端实现音视频的解码流程

在第三期功能中，我们实现了在`Qt`上通过`RTSP/RTMP`链接直接访问摄像头，从而获取视频流的操作。但在真实的工业场景中，这是不允许的，因为客户端无法直接获取摄像头的`IP`地址以及相应的`RTSP/RTMP`链接，这些信息是需要从服务器获取的。当用户登录进入智能家居监控系统后，服务器会自动获取这些摄像头的信息，再展示在`Qt`的`UI界`面上。

**注意： 有部分童靴在Unbuntu22.04上测试时，发现RTSP流无法正常解析，此时需使用RTMP流**。

### 2.2 **流程图**

![image-20250604121454910](./assets/image-20250604121454910.png)



## 3、摄像头基本信息设计

当在界面中双击某个通道的标签时，进行播放。那有一个问题：**如何获取到各个摄像头的信息呢**？

摄像头的信息是需要提前存储到服务器的，因此我们需要设计一张设备表，将所有设备的信息写入其中。

**一旦用户登录到系统之后**，服务器就要**查询数据库加载设备列表的信息**，**存入到内存的一个数据结构（hash）中**。

当Qt客户端的用户成功登录之后，就从服务器的中获取当前系统中的摄像头信息，构成我们看到的设备列表。

![image-20250604105337098](./assets/image-20250604105337098.png)

以上，我们需要设计一张关于**设备表**的信息，其中包含的字段有:

|  id  | type |             serial_no              | channels |       ip        | rtsp | rtmp |
| :--: | :--: | :--------------------------------: | :------: | :-------------: | :--: | :--: |
|  1   |  0   | `09aaa54c30d06875d926a874617c06a5` |    2     | 192.168.106.122 | ...  | ...  |

- type: 0代表枪机， 1代表球机



![image-20250604112750809](./assets/image-20250604112750809.png)

![image-20250604113058109](./assets/image-20250604113058109.png)



## 4、服务器存储录像

当摄像头上线时，服务器要存储摄像头实时流。

录像的存储涉及到录像文件的**元数据**，元数据就是数据的数据，比如该录像文件是哪一个摄像头的，几号通道，文件名，开始时间和结束时间。

|  id  | name | start_time | end_time | camera_id | channel |
| :--: | :--: | :--------: | :------: | :-------: | :-----: |
|      |      |            |          |           |         |

由于实时的视频流没有时间限制，录像要以文件的形式存储下来，每个文件需要**分片存储**，可以设置一个最大值,，比如`100M`，比如`1个G`，或者以时间为单位，比如1个小时。



## 5、客户端回看录像

通过Qt客户端**回看**摄像头录像。

获取录像的参数之后，再进行**文件的读取操作**，然后再发给Qt客户端，流程类似于获取实时流了。







# 五期功能

## 1、集成智慧厨房的功能

**学有余力可以自行完成**。**注意**：

1）硬件**传感器数据**要设计一张表，存入数据库。

2）Qt客户端需要设计**显示**硬件传感器的`UI`界面。

![image-20250603153425158](./assets/image-20250603153425158.png)



## 2、架构图2.0版本

![image-20250604122539150](./assets/image-20250604122539150.png)



## 3、迅思维其他功能实现

参考接口文档，随意发挥。
