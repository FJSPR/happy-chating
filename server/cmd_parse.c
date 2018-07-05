#include"data.h"
#include "cmd_parse.h"
#include "mySQL.h"
int cmd_user(sqlite3 *db,char **errmsg,vpChat temp,int sockfd)//用语判别用户信息
{
	int flag;
	printf("cmd = %d\n",temp->cmd);
   switch(temp->cmd )
   {
    case REG:
		  {
	         flag = read_user(db,errmsg,temp->name);
	         if(flag == USERIN)
	         {
		         return REGNO;//注册重名
	         }
	          else
	         {
                reg_db(db,errmsg,temp->name,temp->passwd);
                return REGOK;
	         }
		     break;
		  }
     case LOG:
		  {
			  int flagpasswd;
			  flag = read_online_ok(db,errmsg,temp->name);
			  if(flag == ONLINEIN)
			  {
				  return ONLINEIN;
			  }
			  else
			  {
				  flagpasswd = read_pass(db,errmsg,temp->name,temp->passwd);
				  if(flagpasswd == PASSWDOK)
				  {
                      log_db(db,errmsg,temp->name,sockfd);
					  return PASSWDOK;
				  }
				  else
				  {
					  return PASSWDNO;
				  }
			  }
			  break;
		  }  
	  case ALL:
		   {
				return ALLOK;
				break;
		   }

   }
}
