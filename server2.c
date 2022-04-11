#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *error_msg)
{
    perror(error_msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
     int socket_fd, newsocket_fd;
     int portno;
     socklen_t cli_len;
     char buffer[256];
     pid_t childpid;
     struct sockaddr_in server_addr, client_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     // create a socket
     // socket(int domain, int type, int protocol)
     socket_fd =  socket(AF_INET, SOCK_STREAM, 0);
     if (socket_fd < 0) 
        error("Socket can't be opened");

     // clear address structure
     bzero((char *) &server_addr, sizeof(server_addr));

     portno = atoi(argv[1]);
     server_addr.sin_addr.s_addr = INADDR_ANY;
     server_addr.sin_family = AF_INET;  
     server_addr.sin_port = htons(portno);

    
     if (bind(socket_fd, (struct sockaddr *) &server_addr,
              sizeof(server_addr)) < 0) 
              error("The socket and server address can't be binded");
     int backlog =1;
     //setting the backlog queue to 10
     listen(socket_fd,10);
     cli_len = sizeof(client_addr);
     while(1){
     
     newsocket_fd = accept(socket_fd, 
                 (struct sockaddr *) &client_addr, &cli_len);
     if (newsocket_fd < 0){
          error("ERROR on accept");
          exit(1);
          }

     printf("Connected with client with port numer %d\n",ntohs(client_addr.sin_port));
     int answer;
     if((childpid = fork()) == 0){
	close(socket_fd);
	     while(1){

	     bzero(buffer,256);

	     n = read(newsocket_fd,buffer,255);
	     
	     if (n < 0) error("ERROR reading from socket");
	     for(int i=0;i<strlen(buffer);i++){
            if(buffer[i]=='+'){
                char num_1=buffer[i-1];
                int int_num_1=num_1-'0';
                char num_2=buffer[i+1];
                int int_num_2=num_2-'0';
                answer=int_num_1+int_num_2;
                break;
            }
            else if(buffer[i]=='*'){
                char num_1=buffer[i-1];
                int int_num_1=num_1-'0';
                char num_2=buffer[i+1];
                int int_num_2=num_2-'0';
                answer=int_num_1*int_num_2;
                break;
        }
            else if(buffer[i]=='/'){
                char num_1=buffer[i-1];
                int int_num_1=num_1-'0';
                char num_2=buffer[i+1];
                int int_num_2=num_2-'0';
                answer=int_num_1/int_num_2;
                break;
            }
            else if(buffer[i]=='-'){
                char num_1=buffer[i-1];
                int int_num_1=num_1-'0';
                char num_2=buffer[i+1];
                int int_num_2=num_2-'0';
                answer=int_num_1-int_num_2;
                break;
            } 
	    }
	    
	    ;
	    int convert_answer = htonl(answer);
	     send(newsocket_fd, &convert_answer, sizeof(convert_answer), 0);
	     printf("Sending reply: %d\n", answer);
	     }
	     ;}
	     
	 }
     close(newsocket_fd);
     close(socket_fd);
     return 0; 
}
