#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<poll.h>
#include<pthread.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#include<arpa/inet.h>
#include<poll.h>

const int client_num = 5;
int sfd[client_num];
int pid[client_num];
int nsfd[client_num];


void* read_thread(void* args){
    int *threadId = (int *)args;
    while(1){
        char buffer[2048];
        read(nsfd[*threadId], buffer, 2048);
        
        // If group chat do
        /*
            for(int i=0;i<client_num;i++){
                if(nsfd[i] != 0){
                    write(nsfd[i], buffer, 2048);
                }
            }

        */

        int recv = atoi(buffer[1]);
        if(nsfd[recv] == 0){
            printf("This client hasnt joined chat yet\n");
        }
        else
            write(nsfd[recv], buffer, 2048);
    }
}



int main(){

    
    int port_num[client_num];
    struct sockaddr_in address[client_num];

    //configure to take this info from config file
    port_num[0] = 6060;
    port_num[1] = 7070;
    port_num[2] = 9090;
    port_num[3] = 8080;

    for(int i=0;i<client_num;i++){
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
    
    struct pollfd fds[client_num];
    memset(fds, 0 , sizeof(fds));

    for(int i=0;i<client_num;i++){
        fds[i].fd = sfd[i];
        fds[i].events |= POLLIN;
    }
    int timeout = (3 * 60 * 1000);
    int connections = 0;

    pthread_t threads[client_num];

    while(connections != client_num){
        int rc = poll(fds, client_num, timeout);
        if(rc < 0){
            perror("poll failed");
            break;
        }
        if (rc == 0){
            printf("  poll() timed out.  End program.\n");
            break;
        }

        for (int i=0;i<client_num;i++){
            if(fds[i].revents == 0)
                continue;

            if(fds[i].revents & POLLIN){
                connections++;
                int addrlen = sizeof(address[i]);
                if((nsfd[i] = accept(sfd[i], (struct sockaddr *)&address[i], (socklen_t*)&addrlen))<0){
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                pthread_create(&threads[i], NULL, read_thread, (void *)&i);
                
                //read and fork and all that
            }
        }
    }
}