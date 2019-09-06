#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>


void print_usage(char * cmd)
{
	fprintf(stderr," %s usage:\n",cmd);
	fprintf(stderr,"%s IP_Addr [port]\n",cmd);

}


int main(int argc,char** argv)
{
	struct sockaddr_in server;
	int ret;
	int len;
	int port;
	int sockfd;
	int sendnum;
	int recvnum;
	char send_buf[2048];
	char recv_buf[2048];
    len=sizeof(server);
	if ((2>argc)|| (argc >3))
	{
		print_usage(argv[0]);
		exit(1);

	}

    if (3==argc) 
    {
		port = atoi(argv[2]);
    }

    if (-1==(sockfd=socket(AF_INET,SOCK_DGRAM,0)))
	{
		perror("can not create socket\n");
		exit(1);
	}

	memset(&server,0,sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(port);

	while(1)
	{
		printf("please enter:\n");
        fflush(stdout);
		ssize_t read_size=read(0,send_buf,sizeof(send_buf)-1);
		if(0<read_size)
		{
			send_buf[read_size-1]='\0';
			sendto(sockfd,send_buf,strlen(send_buf),0,(struct sockaddr*)&server,sizeof(server));
			if (0==strcmp(send_buf,"quit"))
            {
		       perror("the client break the server process\n");
		       close(sockfd);
		       break;
	       }
			printf("server:\n");
			ssize_t recvnum=recvfrom(sockfd,recv_buf,sizeof(recv_buf)-1,0,(struct sockaddr*)&server,&len);
			if(recvnum>0)
			{
				recv_buf[recvnum]='\0';
				printf("%s\n",recv_buf);
			}
			if (0==strcmp(recv_buf,"quit"))
           {
		       perror("the server break the server process\n");
		       close(sockfd);
		       break;
	      }
		}

	}
   return 0;
}
