#include "data.h"
void is_sqlite_ok(int rc)
{
    if(rc == SQLITE_OK)
	{
	   printf("sqlite %d succse \n",__LINE__);
	}
	else
	{
		printf("sqlite %d error\n",__LINE__);
	}
}
void is_sqlite(int rc)  //测试数据库
{
    if(rc == SQLITE_OK)
	{
        printf("sqlite %d succse \n",__LINE__);
	}
	else
	{
		printf("数据库发生错误，请使用SQLITE3 看数据库!\n");
     	printf("sqlite %d error\n",__LINE__);
		exit(1);
	}
}
void open_db(sqlite3 **db)//打开数据库
{
	int rc;
	rc = sqlite3_open("server.db",db);
	is_sqlite(rc);
}
void creat_user_db(sqlite3 *db,char **errmsg)//建立user数据表
{
	int rc;
    rc = sqlite3_exec(db,"create table if not exists user(id integer primary key autoincrement,name text,passwd text)",NULL,NULL,errmsg);
	is_sqlite_ok(rc);
}
void creat_online_db(sqlite3 *db,char **errmsg)//建立online数据表
{
	int rc;
	sqlite3_exec(db,"drop table online",NULL,NULL,errmsg);
    rc = sqlite3_exec(db,"create table online(id integer primary key autoincrement,name text,socket integer,flag integer)",NULL,NULL,errmsg);
	is_sqlite_ok(rc);
}
int read_user(sqlite3 *db,char **errmsg,char *user)//询问有无此用户
{
	int rc;
	int i;
	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from user",-1,&stmt,0); 
    is_sqlite_ok(rc);
	rc = sqlite3_step(stmt);
	int userflag = 1;
    while(rc == SQLITE_ROW)
	{
       userflag = strcmp(user,sqlite3_column_text(stmt,1));
	   if(userflag == 0 )
	   {
			return USERIN;
	   }	
	   rc = sqlite3_step(stmt);
	}
	return USEROUT;
}
void reg_db(sqlite3 *db,char **errmsg,char *name,char *passwd)//注册插入
{
	char sql[1024];
	int rc;
	sprintf(sql,"insert into user(name,passwd)values('%s','%s')",name,passwd);
	rc = sqlite3_exec(db,sql,NULL,NULL,errmsg);
	is_sqlite(rc);
}
int read_online_ok(sqlite3 *db,char **errmsg,char *user)//用于验证在线用户
{
	int rc;
	int i;
	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from online",-1,&stmt,0); 
    is_sqlite_ok(rc);
	rc = sqlite3_step(stmt);
	int userflag = 1;
    while(rc == SQLITE_ROW)
	{
       userflag = strcmp(user,sqlite3_column_text(stmt,1));
	   if(userflag == 0)
	   {
			return ONLINEIN;
	   }	
	   rc = sqlite3_step(stmt);
	}
	return ONLINEOUT;
}
int read_pass(sqlite3 *db,char **errmsg,char *user,char *passwd)//用于验证(用户名和密码)是否可以登陆
{
	int rc;
	int i;
	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from user",-1,&stmt,0); 
    is_sqlite_ok(rc);
	rc = sqlite3_step(stmt);
	int userflag = 1;
	int passwdflag = 1;
    while(rc == SQLITE_ROW)
	{
       userflag = strcmp(user,sqlite3_column_text(stmt,1));
       passwdflag = strcmp(passwd,sqlite3_column_text(stmt,2));
	   if(userflag == 0 && passwdflag == 0)
	   {
			return PASSWDOK;
	   }	
	   rc = sqlite3_step(stmt);
	}
	return PASSWDNO;
}
void write_online_all(sqlite3 *db,char **errmsg,vpChat temp)//向在线用户发送信息
{
	time_t timep;
	time(&timep);
	char *timedata  = ctime(&timep);
	timedata[strlen(timedata)-1]='\0';
	strcpy(temp->time,timedata);

	int rc;
	char fd[100];
	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from online",-1,&stmt,0); 
    is_sqlite_ok(rc);
	rc = sqlite3_step(stmt);
	int userflag = 1;
    while(rc == SQLITE_ROW)
	{
            strcpy(fd,sqlite3_column_text(stmt,2));
			write( atoi(fd),temp,sizeof(stChat));
	        rc = sqlite3_step(stmt);
	}
}
void delete_online_db(sqlite3 *db,char **errmsg,int sockfd)//删除登陆
{
	char sql[1024];
	int rc;
	sprintf(sql,"delete from online where socket = %d",sockfd);
	rc = sqlite3_exec(db,sql,NULL,NULL,errmsg);
	is_sqlite(rc);
}
void log_db(sqlite3 *db,char **errmsg,char *name,int sockfd)//登陆插入
{
	char sql[1024];
	int rc;
	sprintf(sql,"insert into online(name,socket,flag)values('%s',%d,1)",name,sockfd);
	rc = sqlite3_exec(db,sql,NULL,NULL,errmsg);
	is_sqlite(rc);
}