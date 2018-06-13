#define _GNU_SOURCE 1
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>

#define USER_LIMIT 5
#define BUFFER_SIZE 64
#define FD_LIMIT 65535

struct client_data
{
    sockaddr_in address;
    char* write_buf;
    char buf[ BUFFER_SIZE ];
};

int setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

void addfd( int epollfd, int fd, bool enable_et )
{
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN|EPOLLRDHUP|EPOLLERR;
	if( enable_et )
	{
		event.events |= EPOLLET;
	}
	epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
	setnonblocking( fd );
}
int main( int argc, char* argv[] )
{
    if( argc <= 2 )
    {
        printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );

    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );

    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    assert( ret != -1 );

    ret = listen( listenfd, 5 );
    assert( ret != -1 );

    client_data* users = new client_data[FD_LIMIT];
    struct epoll_event events[USER_LIMIT+1];
    int user_counter = 0;
    for( int i = 1; i <= USER_LIMIT; ++i )
    {
        events[i].data.fd = -1;
        events[i].events = 0;
    }
    events[0].data.fd = listenfd;
    events[0].events = EPOLLIN | EPOLLERR;
	int epollfd = epoll_create();
    while( 1 )
    {
        ret = epoll_wait( epollfd,events, user_counter+1, -1 );
        if ( ret < 0 )
        {
            printf( "epoll failure\n" );
            break;
        }
    
        for( int i = 0; i < ret; ++i )
        {
            if( events[i].data.fd == listenfd ) )
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                if ( connfd < 0 )
                {
                    printf( "errno is: %d\n", errno );
                    continue;
                }
                if( user_counter >= USER_LIMIT )
                {
                    const char* info = "too many users\n";
                    printf( "%s", info );
                    send( connfd, info, strlen( info ), 0 );
                    close( connfd );
                    continue;
                }
                user_counter++;
                users[connfd].address = client_address;
                setnonblocking( connfd );
				addfd(epollfd,connfd,false);
                printf( "comes a new user, now have %d users\n", user_counter );
            }
            else if( events[i].events & POLLERR )
            {
                printf( "get an error from %d\n", events[i].data.fd );
                char errors[ 100 ];
                memset( errors, '\0', 100 );
                socklen_t length = sizeof( errors );
                if( getsockopt( events[i].data.fd, SOL_SOCKET, SO_ERROR, &errors, &length ) < 0 )
                {
                    printf( "get socket option failed\n" );
                }
                continue;
            }
            else if( events[i].events & POLLRDHUP )
            {
                users[events[i].data.fd] = users[events[user_counter].fd];
                close( events[i].data.fd );
                events[i] = events[user_counter];
                i--;
                user_counter--;
                printf( "a client left\n" );
            }
            else if( events[i].events & POLLIN )
            {
                int connfd = events[i].data.fd;
                memset( users[connfd].buf, '\0', BUFFER_SIZE );
                ret = recv( connfd, users[connfd].buf, BUFFER_SIZE-1, 0 );
                printf( "get %d bytes of client data %s from %d\n", ret, users[connfd].buf, connfd );
                if( ret < 0 )
                {
                    if( errno != EAGAIN )
                    {
                        close( connfd );
                        users[events[i].data.fd] = users[events[user_counter].fd];
                        events[i] = events[user_counter];
                        i--;
                        user_counter--;
                    }
                }
                else if( ret == 0 )
                {
                    printf( "code should not come to here\n" );
                }
                else
                {
					struct epoll_event event;
                    for( int j = 1; j <= user_counter; ++j )
                    {
                        if( events[j].data.fd == connfd )
                        {
                            continue;
                        }
						event.data.fd = events[j].data.fd;
						event.events = events[j].events&(~EPOLLIN)|EPOLLOUT;
						epoll_ctl( epollfd, EPOLL_CTL_MOD,event.data.fd, &event );
                        users[events[j].fd].write_buf = users[connfd].buf;
                    }
                }
            }
            else if( events[i].events & POLLOUT )
            {
                int connfd = events[i].fd;
                if( ! users[connfd].write_buf )
                {
                    continue;
                }
                ret = send( connfd, users[connfd].write_buf, strlen( users[connfd].write_buf ), 0 );
                users[connfd].write_buf = NULL;
				struct epoll_event event;
				event.data.fd = events[i].data.fd;
				event.events = events[j].events&(~EPOLLOUT)|EPOLLIN;
				epoll_ctl( epollfd, EPOLL_CTL_MOD,event.data.fd, &event );
            }
        }
    }

    delete [] users;
    close( listenfd );
    return 0;
}
