#include "data.h"

void open_db(sqlite3 **db);//打开数据库
void creat_user_db(sqlite3 *db,char **errmsg);//建立user数据表，记录注册用户
void creat_online_db(sqlite3 *db,char **errmsg);//建立online数据表，记录在线用户

void is_sqlite_ok(int rc);//判断数据库操作是否成功

int read_user(sqlite3 *db,char **errmsg,char *user);//询问user表中有无这个名字的用户
void reg_db(sqlite3 *db,char **errmsg,char *name,char *passwd);//注册插入
int read_online_ok(sqlite3 *db,char **errmsg,char *user);//用于验证user是否在线
int read_pass(sqlite3 *db,char **errmsg,char *user,char *passwd);//用于验证(用户名和密码)是否可以登陆
void write_online_all(sqlite3 *db,char **errmsg,vpChat temp);//向在线用户发送信息
void insert_server_db(sqlite3 *db,char *time,char **errmsg);//向server数据库插入数据
void delete_online_db(sqlite3 *db,char **errmsg,int sockfd);//删除登陆
void log_db(sqlite3 *db,char **errmsg,char *name,int sockfd);//登陆插入
void is_sqlite(int rc);  //测试数据库