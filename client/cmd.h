#ifndef CMD_H
#define CMD_H

int analy_cmd(char *cmd);//命令解析
void cmd_reg(int sockfd,vpChat temp);//注册命令
void cmd_help();//帮助命令
void cmd_log(int sockfd,vpChat temp);

#endif