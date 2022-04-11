// importing the neccesary libraries
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_INPUT_SIZE 256
int main(int argc, char *argv[])
{
int sockfd, portnum, n;
struct sockaddr_in server_addr;
char inputbuf[MAX_INPUT_SIZE];
if (argc < 3) {
fprintf(stderr,"usage %s <server-ip-addr> <server-port>\n", argv[0]);
exit(0);
}
portnum = atoi(argv[2]);
/* Create client socket */
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0)
{
fprintf(stderr, "ERROR opening socket\n");
exit(1);
}
/* Fill in server address */
bzero((char *) &server_addr, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(portnum);
/* Connect to server */
if (connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
{

fprintf(stderr, "ERROR connecting\n");
exit(1);
}
printf("Connected to server\n");
do
{
/* Ask user for message to send to server */
printf("Please enter the message to the server: ");
bzero(inputbuf,MAX_INPUT_SIZE);
fgets(inputbuf,MAX_INPUT_SIZE-1,stdin);
/* Write to server */
n = write(sockfd,inputbuf,strlen(inputbuf));
if (n < 0)
{
fprintf(stderr, "ERROR writing to socket\n");
exit(1);
}
/* Read reply */
bzero(inputbuf,MAX_INPUT_SIZE);
int output_num;
n = read(sockfd, &output_num,sizeof(output_num));
if (n < 0)
{
fprintf(stderr, "ERROR reading from socket\n");
exit(1);
}
printf("Server replied: %d\n",ntohl(output_num));
} while(1);
return 0;
}
