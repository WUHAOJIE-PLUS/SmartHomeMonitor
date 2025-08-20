#ifndef __MySQLClient_H__
#define __MySQLClient_H__
#include <mysql/mysql.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace wd{
class MySQLClient {

public:

    MySQLClient();

    ~MySQLClient();


    bool connect(const string & host,

                 const string & user,

                 const string & passwd,

                 const string & db,

                 unsigned short port);

    //写操作封装(INSERT/UPDATE/DELETE)
    bool writeOperationQuery(const string & sql);

    //读操作封装(SELECT) 相当于MYSQL_ROW
    vector<vector<string>> readOperationQuery(const string &sql);
    static void dump(const vector<vector<string>> &res);

private:

    MYSQL  _conn;
};
}//end of namespace wd
#endif


