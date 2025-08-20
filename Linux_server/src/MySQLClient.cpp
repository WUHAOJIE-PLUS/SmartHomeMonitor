#include "MySQLClient.h"
#include <mysql/mysql.h>
#include <iostream>

using std::cout;
using std::endl;

namespace wd{

MySQLClient::MySQLClient()
{
    MYSQL *pconn = mysql_init(&_conn);
    if(!pconn){
        cout << "mysqlclient init error" << endl;
    }
}

MySQLClient::~MySQLClient(){
    mysql_close(&_conn);
}


bool MySQLClient::connect(const string & host,

                          const string & user,

                          const string & passwd,

                          const string & db,

                          unsigned short port){
    cout << "connect" << endl;
    MYSQL *pconn = mysql_real_connect(&_conn, host.c_str(), user.c_str(), passwd.c_str(),db.c_str(), port, nullptr, 0);
    if(!pconn) {
        fprintf(stderr, "%s\n", mysql_error(&_conn));
        return false;
    }
    return true;
}

//写操作封装(INSERT/UPDATE/DELETE)
bool MySQLClient::writeOperationQuery(const string & sql){
    cout << "write" << endl;
    int ret = mysql_real_query(&_conn, sql.c_str(), sql.size());
    if(ret != 0) {
        printf("(%d, %s)\n", mysql_errno(&_conn), mysql_error(&_conn));
        return false;
    } else {
        printf("Query OK, %ld row(s) affected.\n", mysql_affected_rows(&_conn));
        return true;
    }
}

//读操作封装(SELECT)
vector<vector<string> >MySQLClient::readOperationQuery(const string &sql){
    int ret = mysql_real_query(&_conn, sql.c_str(), sql.size());
    if(ret != 0) {
        printf("(%d, %s)\n", mysql_errno(&_conn), mysql_error(&_conn));
        return vector<vector<string>>();
    }

    vector<vector<string>> queryRes;;
    MYSQL_RES * res = mysql_store_result(&_conn);
    if(!res) {
        //没有结果集
        printf("(%d, %s)\n", mysql_errno(&_conn), mysql_error(&_conn));
        return vector<vector<string>>();
    }
    //有结果集的情况
    int rows = mysql_num_rows(res);
    if(rows == 0) {
        return vector<vector<string>>();
    }
    //加一是因为rows不包含第一行
    queryRes.reserve(rows + 1);

    int cols = mysql_num_fields(res);
    printf("rows: %d, cols: %d\n", rows, cols);

    MYSQL_FIELD * filed = nullptr;
    vector<string> fileds;
    //返回采用MYSQL_FIELD结构的结果集的列。重复调用该函数，
    //以检索关于结果集中所有列的信息。未剩余字段时，mysql_fetch_field()返回NULL
    while((filed = mysql_fetch_field(res)) != nullptr) {
        fileds.push_back(filed->name);
    }

    //添加右值是因为，一个query不需要第二此，所以利用右值直接让filed指向nullptr
    queryRes.push_back(std::move(fileds));//转移到容器中

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

    mysql_free_result(res);
    return queryRes;
}

//对结果集进行打印操作
void MySQLClient::dump(const vector<vector<string>> & res)
{
    cout << "result set has " << res.size() << " row(s)" << endl;
    //一行一行存的
    for(auto & vec : res) {
        for(auto & filed : vec) {
            cout << filed << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

}//end of namespace wd


