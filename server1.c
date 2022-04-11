#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
// for printing the error
void error(const char *error_msg)
{
    perror(error_msg);
    exit(EXIT_FAILURE);
}
// making the main function 
// argv[] contains the Port number and ip address
int main(int argc, char *argv[])
{
     int socket_fd, newsocket_fd;
     int portno;
     socklen_t cli_len;
     char buffer[256];
     struct sockaddr_in server_addr, client_addr;
     int n;
     // checking for a valid port number
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     // creating a socket
     socket_fd =  socket(AF_INET, SOCK_STREAM, 0);
     if (socket_fd < 0) 
        error("Socket can't be opened");

     // clearing the address structure
     bzero((char *) &server_addr, sizeof(server_addr));
     //getting the port number
     portno = atoi(argv[1]);

     server_addr.sin_port = htons(portno);
     server_addr.sin_family = AF_INET;  
     server_addr.sin_addr.s_addr = INADDR_ANY;  
    // binding the socket to the server adress
     if (bind(socket_fd, (struct sockaddr *) &server_addr,
              sizeof(server_addr)) < 0) 
              error("The socket and server address can't be binded");


     if(listen(socket_fd,1)<0){
         error("Line,Busy!");
     }

     cli_len = sizeof(client_addr);
     newsocket_fd = accept(socket_fd, 
                 (struct sockaddr *) &client_addr, &cli_len);
     if (newsocket_fd < 0) 
          error("The accept connection culdn't happened");

     printf("Connected with client with port Number: %d\n",ntohs(client_addr.sin_port));
     int answer;
     do{
     
     // making all the thing zeros in the adress if present

     bzero(buffer,256);

     n = read(newsocket_fd,buffer,255);
     
     if (n < 0) error("The socket can't be readed");
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
    
     int converted_answer = htonl(answer);
     send(newsocket_fd, &converted_answer, sizeof(converted_answer), 0);     
     }while(1);
     printf("Sending reply: %d\n", answer);
     close(newsocket_fd);
     close(socket_fd);
     return 0; 
}
