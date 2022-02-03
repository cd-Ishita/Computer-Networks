#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    

    // Error checking omitted for expository purposes
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);


    // Connect to the remote server
    struct sockaddr_in remoteaddr;
    remoteaddr.sin_family = AF_INET;
    remoteaddr.sin_addr.s_addr = inet_addr("10.0.2.15");
    remoteaddr.sin_port = htons(8000);
    connect(sockfd, (struct sockaddr *)&remoteaddr, sizeof(remoteaddr));
    
    /*
    char buf[2048];
    printf("Client 1 wants input: \n");
    scanf("%s", buf);
    send(sfd, buf, 2048, 0);
    printf("client1: sent '%s'\n",buf);
*/
    char message_recv[2048];
    int bytes_recv = recv(sockfd, message_recv, 2047, 0);
    message_recv[bytes_recv] = '\0';
    printf("Message received is: %s\n", message_recv);

    close(sockfd);

    return 0;
}