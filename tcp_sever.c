#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<pthread.h>
#include<sys/wait.h>
#define _BACK_LOG 5
int starup(const char* ip,unsigned short port)

{
   int listen_sock = socket(AF_INET,SOCK_STREAM,0);//create socket

 if(listen_sock == -1)
 {
       printf("%d : %s\n",errno,strerror(errno));
       exit(1);
 }
  printf("%d\n",listen_sock);
  
  struct sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_port = htons(port);
  inet_aton(ip,&local.sin_addr);
  if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local))==-1)//  bind
    {
		printf("%d:  %s\n",errno,strerror(errno));
	   exit(1);
     }
  if(listen(listen_sock,_BACK_LOG)==-1)
      {
		  printf("%d: %s\n",errno,strerror(errno));
		  exit(1);
  	  }

      return listen_sock;

}

static void wait_child(int signo)
{
	pid_t exit_id;
	while((exit_id = waitpid(-1,NULL,WNOHANG))>0)
	{printf("child exit,pd is: %d\n,exit_id");
	}
}

void *thread_run(void *arg)
{
	printf("create a new pthread\n");
	int new_client = (int)arg;
      char buf[1024];
      while(1)
     {
		 memset(buf,'\0',sizeof(buf));
        ssize_t size=read(new_client,buf,sizeof(buf)-1);
        if(size<0)
         {
			 printf("read error\n");	
			 break;
		 }
      else if(size == 0)
        {
			printf("connect realse\n");
			break;
		}
        else{
			buf[size]='\0';
			printf("client say: %s ",buf);
            }   
	 }
 close(new_client);
}
void usage(const char* proc)
{
  printf("Usage: %s[IP][PORT]\n",proc);

}

int main(int argc,char* argv[])
{
    if(argc != 3)
	{
      usage(argv[0]);
	  exit(1);
    }
	signal(SIGCHLD,wait_child);
	char* ip= argv[1];
	int port = atoi(argv[2]);
	struct sockaddr_in client_info;
	socklen_t client_len =0;
    int listen_sock=starup(ip,port);
   while(1)
   {
       int new_client = accept(listen_sock,(struct sockaddr*)&client_info,&client_len);
       if(new_client<0)
         {
			 printf("accept error\n");
			 continue;
		 }
	   printf("get a new connect...!!!");
#ifdef _FUN1

      char buf[1024];
	  printf("using fun1!\n");
      while(1)
     {
		 memset(buf,'\0',sizeof(buf));
        ssize_t size=read(new_client,buf,sizeof(buf)-1);
        if(size<0)
         {
			 printf("read error\n");	
			 break;
		 }
      else if(size == 0)
        {
			printf("connect realse\n");
		}
        else{
			buf[size]='\0';
			printf("client say: %s ",buf);
            }    

}
#endif
#ifdef _FUN2
printf("using fun2...\n");
pid_t id = fork();
if (id<0)
{
	perror("fork");
}
else if(id == 0)
{
	close(listen_sock);
	char buf[1024];
	while(1)
	{
		memset(buf,'\0',sizeof(buf)-1);
		ssize_t size = read(new_client,buf,sizeof(buf)-1);
		if(size<0)
		{perror("read ");
		}
		else if(size == 0)
		{printf("client realse\n");
			break;
		}
		else
		{
			buf[size]='\0';
			printf("client say: %s",buf);
		}
		close(new_client);
		exit(1);


	}
	close(new_client);
}
#endif
#ifdef _FUN3
printf("FUN3...\n");
pthread_t t_id;
pthread_create(&t_id,NULL,thread_run,(void*)new_client);
pthread_detach(t_id);

#endif
}
    return 0;

}
