#include"data.h"
#include "cmd.h"
#include "my_function.h"
int analy_cmd(char *cmd)//命令解析
{
   if(strcmp(cmd,"reg") == 0)
   {
	   return REG;
   }
   else if(strcmp(cmd,"log") == 0)
   {
	   return LOG;
   }
   else if(strcmp(cmd,"help") == 0)
   {
       return HELP;
   }
   else if(strcmp(cmd,"exit") == 0)
   {
       return EXIT;
   }
   else
   {
	   return ERROR;
   }
}

void cmd_reg(int sockfd,vpChat temp)
{
   char passwd[20];
   char passwd1[20];
   printf("请输入用户名：");
   scanf("%s",temp->name);
   printf("请输入密码：");
   scanf("%s",passwd);
   printf("请确认密码：");
   scanf("%s",passwd1);
   if(strcmp(passwd,passwd1) == 0)
   {
      
      printf("正在注册，请稍等！\n");
      strcpy(temp->passwd,passwd);
      printf("发送注册信息\n");
      mywrite(sockfd,temp);
      memset(temp,0,sizeof(stChat));
      temp->revert = -1;
      read(sockfd,temp,sizeof(stChat));
      if(temp->revert == REGOK)
      {
         sleep(1);
         printf("您已注册成功,欢迎您的使用！\n");
         printf("\n");
      }
      else if(temp->revert == REGNO)
      {
         sleep(1);
         printf("\n");
         printf("已有账户注册过此用户名！");
         printf("\n");
      }
      else
      {
         sleep(1);
         printf("\n");
         printf("服务器未向您发送确定信息！");
         printf("\n");
      }
   }
   else
   {
      printf("\n");
      printf("您输入的密码不一致！");
   }
}
void cmd_help()
{
    printf("**********欢迎使用聊天室**********\n");
    printf("         1、注册请输入reg         \n");
    printf("         2、登录请输入log       \n");
    printf("         3、帮助信息help         \n");
    printf("         4、放弃登录exit         \n");
}
void cmd_log(int sockfd,vpChat temp)
{
   printf("请输入用户名：");
   scanf("%s",temp->name);
   printf("\n请输入密码：");
   char passwd[20];
   scanf("%s",passwd);
   printf("\n");
   strcpy(temp->passwd,passwd);
   mywrite(sockfd,temp);
   printf("\n");
   printf("正在登录，请稍等！\n");
}