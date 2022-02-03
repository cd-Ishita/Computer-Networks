#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<poll.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main(){

    //Open 4 sfd
    // First 3 sfd are TCP
    // last sfd is UDP
    int num = 4; // Number of sfd needed
    
    int sfd[num];
    int port_num[num];
    struct sockaddr_in address[num];

    //configure to take this info from config file
    port_num[0] = 6060;
    port_num[1] = 7070;
    port_num[2] = 9090;
    port_num[3] = 8085;

    for(int i=0;i<num-1;i++){
        int opt = 1;
        int addrlen = sizeof(address[i]);
        
        // Creating socket file descriptor
        if ((sfd[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        
        // Forcefully attaching socket to the port 8080
        if (setsockopt(sfd[i], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address[i].sin_family = AF_INET;
        address[i].sin_addr.s_addr = INADDR_ANY;
        address[i].sin_port = htons(port_num[i]);
        
        // Forcefully attaching socket to the port 8080
        if (bind(sfd[i], (struct sockaddr *)&address[i], sizeof(address[i]))<0){
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(sfd[i], 5) < 0){
            perror("listen");
            exit(EXIT_FAILURE);
        }
        printf("sfd%d is listening for connections\n", i+1);
    }
    
    // 1 UDP connection, dgram connectionless
    
    struct sockaddr_in servaddr, cliaddr;
       
    // Creating socket file descriptor
    if ( (sfd[num-1] = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
       
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
       
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port_num[num-1]);
       
    // Bind the socket with the server address
    if ( bind(sfd[num-1], (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("sfd%d is ready for connections\n", num);

    // ----------------------------------------- //
    // -------- POLLING -------------//
    struct pollfd fds[num];
    int nsfd[num];
    memset(fds, 0 , sizeof(fds));

    for(int i=0;i<num;i++){
        fds[i].fd = sfd[i];
        fds[i].events |= POLLIN;
    }
    int timeout = (3 * 60 * 1000);
    int connections = 0;
    while(connections != num){
        int rc = poll(fds, num, timeout);
        if(rc < 0){
            perror("poll failed");
            break;
        }
        if (rc == 0){
            printf("  poll() timed out.  End program.\n");
            break;
        }

        for (int i=0;i<num;i++){
            if(fds[i].revents == 0)
                continue;

            if(fds[i].revents & POLLIN){
                connections++;
                if(i == num-1){
                    char buffer[2048];
                    struct sockaddr_in cliaddr;
                    int len = sizeof(cliaddr);
                    int n = recvfrom(sfd[i], (char *)buffer, 2048, 
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                    &len);

                    printf("Message for sfd%d is %s\n", i, buffer);
                }
                else{
                    int addrlen = sizeof(address[i]);
                    if((nsfd[i] = accept(sfd[i], (struct sockaddr *)&address[i], (socklen_t*)&addrlen))<0){
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    char buffer[1024] = {0};
                    read(nsfd[i], buffer, 1024);
                    printf("Message for sfd%d is %s\n", i, buffer);
                    //read and fork and all that
                }
            }
        }
    }
}