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
#include <wait.h>
int main(){

    // Setup client sockets
    int num = 5;
    
    int sfd[num];
    int port_num[num];
    struct sockaddr_in address[num];

    //configure to take this info from config file
    port_num[0] = 8080;
    port_num[1] = 8081;
    port_num[2] = 8082;
    port_num[3] = 8083;
    port_num[4] = 8084;

    for(int i=0;i<num;i++){
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
    
    // Set up data server sockets
    int data_num = 4;
    
    int data_sfd[num];
    int data_port_num[num];
    struct sockaddr_in data_address[num];

    //configure to take this info from config file
    data_port_num[0] = 10000;
    data_port_num[1] = 10001;
    data_port_num[2] = 10002;
    data_port_num[3] = 10003;

    for(int i=0;i<data_num;i++){
        int opt = 1;
        int addrlen = sizeof(data_address[i]);
        
        // Creating socket file descriptor
        if ((data_sfd[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        
        // Forcefully attaching socket to the port 8080
        if (setsockopt(data_sfd[i], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        data_address[i].sin_family = AF_INET;
        data_address[i].sin_addr.s_addr = INADDR_ANY;
        data_address[i].sin_port = htons(data_port_num[i]);
        
        // Forcefully attaching socket to the port 8080
        if (bind(data_sfd[i], (struct sockaddr *)&data_address[i], sizeof(address[i]))<0){
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(data_sfd[i], 5) < 0){
            perror("listen");
            exit(EXIT_FAILURE);
        }
        printf("data sfd%d is listening for connections\n", i+1);
    }
    
    // ----------------------------------------- //
    // -------- POLLING -------------//
    struct pollfd fds[num];
    int nsfd;
    memset(fds, 0 , sizeof(fds));

    for(int i=0;i<num;i++){
        fds[i].fd = sfd[i];
        fds[i].events |= POLLIN;
    }
    int timeout = (3 * 60 * 1000);
    int connections = 0;

    int data_opened[data_num];
    for(int i=0;i<data_num;i++){
        data_opened[i] = 0;
    }

    int data_client[data_num][10];

    int data_server_nsfd[data_num];
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
                int addrlen = sizeof(address[i]);
                if((nsfd = accept(sfd[i], (struct sockaddr *)&address[i], (socklen_t*)&addrlen))<0){
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                char buffer[1024] = {0};
                read(nsfd, buffer, 1024);
                printf("Message for sfd%d is %s\n", i, buffer);
                //read and fork and all that

                int server_num = 0;
                if(buffer[i] == '1'){
                    server_num = 0;
                }
                else if(buffer[i] == '2'){
                    server_num = 1;
                }
                else if(buffer[1] == '3'){
                    server_num = 2;
                }
                else{
                    server_num = 3;
                }

                data_client[server_num][0] = nsfd;
                if(data_opened[server_num] == 0){
                    printf("Here");
                    int c = fork();
                    if(c > 0){
                        printf("here");
                        int addrlen = sizeof(data_address[server_num]);
                        data_server_nsfd[server_num] = accept(data_sfd[server_num], (struct sockaddr *)&data_address[i], (socklen_t*)&addrlen);

                        data_opened[server_num] == 1;
                        printf("Data server connection established %d", server_num);
                    }
                    else{
                        printf("there");
                        char *args = {"./d1", NULL};
                        execv(args[0], args);
                    }
                }
                
            }
        }
    }
}