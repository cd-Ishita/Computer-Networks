#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"  // the port users will be connecting to

void sigchld_handler(int s){
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


int main(void){
    
    int sfd;
    int port_num;
    struct sockaddr_in address;

    //configure to take this info from config file
    port_num = 6060;
    int opt = 1;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Forcefully attaching socket to the port 8080
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_num[i]);
    
    // Forcefully attaching socket to the port 8080
    if (bind(sfd, (struct sockaddr *)&address, sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(sfd, 5) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("sfd%d is listening for connections\n", i+1);

    // Find IP Address
    struct sockaddr_in address;
    int address_len = sizeof(address);
    int x = getsockname(sfd, &address, &address_len);

    printf("Local IP address is: %s\n", inet_ntoa(address.sin_addr));
    printf("Local port is: %d\n", (int) ntohs(address.sin_port));


    //Now accept
    struct sockaddr_storage client_address; // connector's address information
    socklen_t sin_size;
    int new_fd; //new connection
    char s[INET6_ADDRSTRLEN];

    while(1) {  // main accept() loop
        sin_size = sizeof client_address;
        new_fd = accept(sfd, (struct sockaddr *)&client_address, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        /*
        inet_ntop(client_address.ss_family,
            get_in_addr((struct sockaddr *)&client_address),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
        */
        //CHECK IF WE NEED THIS

        socklen_t len;
        struct sockaddr_storage addr;
        char ipstr[INET6_ADDRSTRLEN];
        int port;

        len = sizeof addr;
        getpeername(sfd, (struct sockaddr*)&addr, &len);

        // deal with both IPv4 and IPv6:
        if (addr.ss_family == AF_INET) {
            struct sockaddr_in *sfd = (struct sockaddr_in *)&addr;
            port = ntohs(sfd->sin_port);
            inet_ntop(AF_INET, &sfd->sin_addr, ipstr, sizeof ipstr);
        } else { // AF_INET6
            struct sockaddr_in6 *sfd = (struct sockaddr_in6 *)&addr;
            port = ntohs(sfd->sin6_port);
            inet_ntop(AF_INET6, &sfd->sin6_addr, ipstr, sizeof ipstr);
        }

        printf("Peer IP address: %s\n", ipstr);
        printf("Peer port      : %d\n", port);

        char message_recv[2048];
        int bytes_recv = recv(new_fd, message_recv, 2047, 0);
        message_recv[bytes_recv] = '\0';
        printf("Message received is: %s\n", message_recv);

        if (!fork()) { // this is the child process
            close(sfd); // child doesn't need the listener
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }


    return 0;
}