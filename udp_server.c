#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>

static void usage(char *argv)
{
	printf("Usage:%s [local ip] [local]",argv);
}

int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		usage(argv[0]);
		return 1;
	}
	int sock=socket(AF_INET,SOCK_DGRAM,0);
	if(sock<0){
		perror("socket");
		return 2;
	}
	
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(atoi(argv[2]));
	local.sin_addr.s_addr=inet_addr(argv[1]);
	char buf[1024];
	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
		perror("bind");
		return 3;
	}

	struct sockaddr_in client;
	socklen_t len=sizeof(client);
	while(1)
	{
	
	ssize_t s=recvfrom(sock,buf,sizeof(buf)-1,0,\
			(struct sockaddr*)&client,&len);
	if(s>0){
		buf[s]=0;
		printf(" %s:%d client say# %s\n",inet_ntoa(client.sin_addr)\
				,ntohs(client.sin_port),buf);
		sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&client\
				,len);
	}


	}

	close(sock);
	return 0;
}
