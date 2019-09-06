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
	fprintf(stderr,"%s  [port]\n",cmd);

}

int main(int argc,char** argv)
{
	struct sockaddr_in server;
	struct sockaddr_in client;
	int len;
	int port;
	int sockfd;
	int connectd;
	int sendnum;
	int opt;
	int recvnum;
	char send_buf[2048];
	char recv_buf[2048];
	memset(send_buf,0,2048);
	memset(recv_buf,0,2048);
	
      opt = SO_REUSEADDR;
      len=sizeof(client);
      sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if ((2>argc)|| (argc >2))
	{
		print_usage(argv[0]);
		exit(1);

	}

    if (2==argc) 
    {
		port = atoi(argv[1]);
    }
     if (-1==sockfd)
      {
	   perror("create socket failed\n");
	   exit(-1);
      }
	  setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
     
 
     memset(&server,0,sizeof(struct sockaddr_in));
     server.sin_family = AF_INET;
     server.sin_addr.s_addr = htonl(INADDR_ANY);
     server.sin_port = htons(port);

     if (-1==bind(sockfd,(struct sockaddr *)&server,sizeof(struct sockaddr)))
     {
	     perror("bind error\n");
	    exit(1);

     }

    while (1)
    {
   	  #ifdef DEBUG
		printf("the connect id is %d",connectd);
		printf("the client ip addr is %s",inet_ntoa(client.sin_addr));
 	  #endif
      printf("\nclient:\n");
	  recvnum=recvfrom(sockfd,recv_buf,sizeof(recv_buf)-1,0,(struct sockaddr *)&client,(socklen_t*)&len);

	  if (0>recvnum)
	  {
		 perror("recv error");
		 exit(1);
		 continue;
	  }else
	  {
		 recv_buf[recvnum]='\0';
		 printf("%s",recv_buf);
		if (0==strcmp(recv_buf,"quit"))
        {
		  perror("the client break the server process\n");
		  close(sockfd);
		  break;
	    }
	
		sendto(sockfd,recv_buf,strlen(recv_buf),0,(struct sockaddr*)&client,sizeof(client));
		if (0==strcmp(recv_buf,"quit"))
        {
		    perror("the server break the server process\n");
		    close(sockfd);
		    break;
	    }
		 		
	  }
	  
	  

	 
	   #ifdef DEBUG
		printf("the send num is %d",sendnum);
		printf("the client ip addr is %s",inet_ntoa(client.sin_addr));
   	   #endif

   }

    return 0;

}

