#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <netdb.h> 


int main(int argc,char *args[])
{
    char *host = args[1];
    int portno = atoi(args[2]);
    char *params = args[3];
    
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total, message_size;
    char *message, response[10000];
    message=malloc(10000);
    sprintf(message,"GET %s HTTP/1.0\r\n", params);            
    strcat(message,"\r\n");
    printf("%s", message);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname(host);
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);
    connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);
    memset(response,0,sizeof(response));
    total = sizeof(response)-1;
    received = 0;
    do {
        bytes = read(sockfd,response+received,total-received);

        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);
    close(sockfd);
    printf("%s\n", response);
    free(message);
    return 0;
}