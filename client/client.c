#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include "data.h"
#include "cmd.h"
#include "my_function.h"

#define BUFFER_SIZE 64

int main( int argc, char* argv[] )
{
    if( argc <= 2 )
    {
        printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );

    struct sockaddr_in server_address;
    bzero( &server_address, sizeof( server_address ) );
    server_address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &server_address.sin_addr );
    server_address.sin_port = htons( port );

    int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( sockfd >= 0 );
    if ( connect( sockfd, ( struct sockaddr* )&server_address, sizeof( server_address ) ) < 0 )
    {
        printf( "connection failed\n" );
        close( sockfd );
        return 1;
    }
    struct pollfd fds[2];
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    fds[1].fd = sockfd;
    fds[1].events = POLLIN | POLLRDHUP;
    fds[1].revents = 0;
    char read_buf[BUFFER_SIZE];
    int pipefd[2];
    int ret = pipe( pipefd );
    assert( ret != -1 );
 
    char cmd[100];
    bool loged = false;
    char name[20];
    stChat temp;
    //登录/注册阶段，登录之后才可以进入聊天阶段；
    while(!loged)
    {
        printf("\t请输入相应的命令(如忘记请输help):");
        scanf("%s",cmd);
        temp.cmd = analy_cmd(cmd);
        switch(temp.cmd)
        {
            case REG:
                  {
                     cmd_reg(sockfd,&temp);
                     break;
                  }
            case LOG:
                  {
                        cmd_log(sockfd,&temp);  
                        strcpy(name,temp.name);
                        memset(&temp,0,sizeof(stChat));
                        read(sockfd,&temp,sizeof(stChat));
                        if(temp.revert == PASSWDOK)
                        {
                             sleep(1);
                             printf("您已登录成功,欢迎您的使用！\n");
                             sleep(1);
                             loged = true;
                        }
                        else if(temp.revert == ONLINEIN)
                        {
                             sleep(1);
                             printf("该用户已经登陆！\n");
                        }
                        else if(temp.revert == PASSWDNO)
                        {
                             sleep(1);
                             printf("您输入的密码有误或者用户不存在！\n");
                        }
                        else
                        {
                            sleep(1);
                            printf("\t\t\t\t\t服务器未向您发送确定信息！\n");
                        }
                        break;
                  }
            case HELP:
                  {
                    cmd_help();
                    break;
                  }
            case EXIT:
                  {
                      exit(1);
                  }
            default:
                  {
                      printf("您输入的命令有误！\n");
                      sleep(1);
                  }
        }
    }
    //聊天阶段
    temp.cmd = ALL;
    int n;
    while( 1 )
    {
        ret = poll( fds, 2, -1 );
        if( ret < 0 )
        {
            printf( "poll failure\n" );
            break;
        }

        if( fds[1].revents & POLLRDHUP )
        {
            printf( "server close the connection\n" );
            break;
        }
        else if( fds[1].revents & POLLIN )
        {
            memset((temp.msg),0,sizeof(800));
            read(sockfd,&temp,sizeof(stChat));
            printf( "%s\n%s : %s\n", temp.time,temp.name,temp.msg );
        }

        if( fds[0].revents & POLLIN )
        {
            //read时，会把换行符也读进来
            n = read(0,temp.msg,800);
            temp.msg[n-1]='\0';
            strcpy(temp.name,name);//由于每次接收数据时会把name覆盖，所以发送前需要重新写入用户名
            mywrite(sockfd,&temp);
        }
    }
    
    close( sockfd );
    return 0;
}
