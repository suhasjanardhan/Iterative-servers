#include <stdio.h>
#define _USE_BSD 1
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>

#define MAX_SEND_BUF 1000
#define MAX_RECV_BUF 1000
#define MAX_DATA 1000


int connectTCP(const char *service,int portnum);
int connectsock(const char *service,int portnum,const char *transport);


/*------------------------------------------------------------------------------------
 * connectsock-Allocate and connect socket for TCP
 *------------------------------------------------------------------------------------
*/

int connectsock(const char *service,int portnum,const char *transport)
{
/*
Arguments:
*service   - service associated with desired port
*transport - name of the transport protocol to use
*/
struct sockaddr_in server;                                                //an internet endpoint address

int server_socket,type,b,l,accept_socket,num;                             //two socket descriptors for listening and accepting 

memset(&server,0,sizeof(server));

server.sin_addr.s_addr=htons(INADDR_ANY);                                 //INADDR_ANY to match any IP address
server.sin_family=AF_INET;                                                //family name
server.sin_port=htons(portnum);                                              //port number

 
/*
 * to determine the type of socket
 */

if(strcmp(transport,"udp")==0)
{
type=SOCK_DGRAM;
}
else
{
type=SOCK_STREAM;
}


server_socket=socket(AF_INET,type,0);                                    //allocate a socket

if(server_socket<0)
{
printf("Socket can't be created\n");
exit(0);
}

/* to set the socket options- to reuse the given port multiple times */
num=1;

if(setsockopt(server_socket,SOL_SOCKET,SO_REUSEPORT,(const char*)&num,sizeof(num))<0)
{
printf("setsockopt(SO_REUSEPORT) failed\n");
exit(0);
}


/* bind the socket to known port */
b=bind(server_socket,(struct sockaddr*)&server,sizeof(server));

if(b<0)
{
printf("Error in binding\n");
exit(0);
}


/* place the socket in passive mode and make the server ready to accept the requests and also 
   specify the max no. of connections
 */
l=listen(server_socket,10);
if(l<0)
{
printf("Error in listening\n");
exit(0);
}

return server_socket;

}



/*------------------------------------------------------------------------
 * connectTCP-connect to a specified TCP service on specified host
 -------------------------------------------------------------------------*/
int connectTCP(const char *service,int portnum)
{
/*
 Arguments:
 *service-service associated with desired port
 */
 return connectsock(service,portnum,"tcp");
}


/*
 
 */

int main(char argc,char *argv[])
{

char msg[1000];
char *service="echo";
int portnum=atoi(argv[1]);


int accept_socket,sock,data_len;
struct sockaddr_in fsin;

/* call connectTCP to create a socket, bind it and place it in passive mode
   once the call returns call accept on listening socket to accept the incoming requests
 */



sock=connectTCP(service,portnum);

printf("Listening to client\n");

while(1)
{

accept_socket=accept(sock,(struct sockaddr*)NULL,NULL);                                           //continously call accept to get tnew requests 

if(accept_socket<0)
{
printf("error in accepting\n");
exit(0);
}



                        char send_buf[MAX_SEND_BUF];



		
			data_len = recv(accept_socket,msg,MAX_DATA,0);                              //recieve the file name from client
			
			
			if(data_len)
			{
				printf("Client connected to Iterative connection oriented server\n");
				printf("File name recieved: %s\n", msg);
				
			}
						
			
			int file;                                                                   //for reading local file(server file)
			if((file = open(msg,O_RDONLY))<0)  
			{       
				
				printf("File not found\n");
				printf("Client disconnected\n");
			}
			else
			{	
				
				printf("File opened successfully\n");
					
						
		

				
				
				ssize_t read_bytes;
 				ssize_t sent_bytes;
				 
				char send_buf[MAX_SEND_BUF]; 
				
	
				 //printf("Sending file: %s\n", data);
				 while( (read_bytes = read(file, send_buf, MAX_RECV_BUF)) > 0 )              //read the contents of file
				 {
					 printf("%s",send_buf);
					 if( (sent_bytes = send(accept_socket, send_buf, read_bytes, 0)) < read_bytes )   //sending the contents of file to client
					 {
					 printf("send error");
					 return -1;
					 }
					 
				 }
				 close(file);
                            printf("\nClient disconnected\n");			 
			}
		 
		
		
		close(accept_socket);                                                                     //closing the connected socket

}

return 0;

}












