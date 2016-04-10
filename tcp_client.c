
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#define _REMOTE_PORT 8080

static void usage(const char *proc)
{
	printf("Usage: %s [IP]\n",proc);
}

int main(int argc,char* argv[])
{
    if(argc != 2)
	{
		usage(argv[0]);
		exit(1);
	}
	const char* ip = argv[1];
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1){
	printf("%d: %s",errno,strerror(errno));
	exit(1);
	}

	unsigned short port = _REMOTE_PORT;
	struct sockaddr_in remote;
	remote.sin_family = AF_INET;
	remote.sin_port = htons(port);
	inet_aton(ip,&remote.sin_addr);
	int acpt = connect(sock,(struct sockaddr*)&remote,sizeof(remote));
	if(acpt<0)
	{
		perror("connect...\n");
		exit(1);
	}

	char buf[1024];
	memset(buf,'\0',sizeof(buf));
	while(1)
	{
		size_t ret = read(0,buf,sizeof(buf)-1);
		if(ret<0)
		{
			perror("read.");
			break;
		}
		buf[ret]='\0';
		ssize_t size = write(sock,buf,strlen(buf));
		if(size<0)
		{
			printf("write");
			break;
		}
	}
	close(sock);
	return 0;
}
