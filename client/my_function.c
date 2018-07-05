#include "my_function.h"
#include "data.h"
void mywrite(int sockfd,vpChat temp)
{
	int num;
    if((num = write(sockfd,temp,sizeof(stChat))) == -1)
    {
        printf("send error!\n");
	}
}