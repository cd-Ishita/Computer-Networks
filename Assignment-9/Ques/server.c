// data server files stores path of executable files
// nsfd data stores the client nsfd for every specific data server
// data server sfd stores nsfd of the data server to this main server



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
#include<pthread.h>
// If a client wants to connect to D1, it needs to connect to port 8081
#define port1 8081
#define port2 8082
#define port3 8083
#define num_data 4

char data_server_files[num_data][2048] = {"./d1", "./d2", "./d3", "./d4"};

int nsfd_data[num_data][10];

int data_server_sfd[num_data];

void* reading(void *args){
    //function to keep reading from specific data server and write to nsfd
    while(1){
        int *data_num = (int *)args;
        char buffer[1024] = {0};
        read(data_server_sfd[*data_num], buffer, 1024);
        
        int i=0;
        while(nsfd_data[*data_num][i] != 0){
            write(nsfd_data[*data_num][i], buffer, 1024);
            i++;
        }
    }
}


int main(){

    // for client sockets
    int num = 5; // Number of sfd needed
    
    int sfd[num];
    int port_num[num];
    struct sockaddr_in address[num];

    //configure to take this info from config file
    port_num[0] = port1;
    port_num[1] = port2;
    port_num[2] = port3;

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
    
    
    // for data server sockets
    int data_socket[num_data];
    int port_data[num_data];
    struct sockaddr_in data_address[num_data];

    //configure to take this info from config file
    port_data[0] = 10000;
    port_data[1] = 10001;
    port_data[2] = 10002;
    port_data[3] = 10003;

    for(int i=0;i<num_data-1;i++){
        int opt = 1;
        int addrlen = sizeof(data_address[i]);
        
        // Creating socket file descriptor
        if ((data_socket[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        
        // Forcefully attaching socket to the port 8080
        if (setsockopt(data_socket[i], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        data_address[i].sin_family = AF_INET;
        data_address[i].sin_addr.s_addr = INADDR_ANY;
        data_address[i].sin_port = htons(port_data[i]);
        
        // Forcefully attaching socket to the port 8080
        if (bind(data_socket[i], (struct sockaddr *)&data_address[i], sizeof(data_address[i]))<0){
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(data_socket[i], 5) < 0){
            perror("listen");
            exit(EXIT_FAILURE);
        }
    }
    

    printf("Start polling\n");
    // ----------------------------------------- //
    // -------- POLLING -------------//
    
    printf("Start polling\n");
    struct pollfd fds[num];
    int nsfd[num];
    memset(fds, 0 , sizeof(fds));


    printf("Start polling\n");
    for(int i=0;i<num;i++){
        fds[i].fd = sfd[i];
        fds[i].events |= POLLIN;
    }
    int timeout = (3 * 60 * 1000);
    int connections = 0;

    printf("Start polling\n");
    // data servers information is already here

    int nsfd_data[num_data][10];
    for(int i=0;i<num_data;i++){
        for(int j=0;j<10;j++){
            nsfd_data[i][j] = 0;
        }
    }

    printf("Start polling\n");
    for(int i=0;i<num_data;i++){
        data_server_sfd[i] = 0;
    }
    pthread_t read_threads[num_data];

    printf("here");
    while(connections != num){
        printf("there");
        int rc = poll(fds, num, timeout);
        printf("bs");
        if(rc < 0){
            perror("poll failed");
            break;
        }
        if (rc == 0){
            printf("  poll() timed out.  End program.\n");
            break;
        }

        for (int i=0;i<num;i++){
            printf("hello");
            if(fds[i].revents == 0)
                continue;

            if(fds[i].revents & POLLIN){
                connections++;
            
                int addrlen = sizeof(address[i]);
                if((nsfd[i] = accept(sfd[i], (struct sockaddr *)&address[i], (socklen_t*)&addrlen))<0){
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                char buffer[1024] = {0};
                read(nsfd[i], buffer, 1024);
                
                //buffer structure is: "D1"
                int req=0;
                for(int it = 1;it<=num_data;it++){
                    if(buffer[1] == (char)(it+48)){
                        req = it;
                    }
                }

                if(req == 0){
                    printf("Wrong buffer structure!");
                }
                else{

                    if(data_server_sfd[req-1] == 0){
                        printf("Data server isnt open\n");
                        // This means that this particular data server isn't open yet
                        int c = fork();
                        if(c > 0){

                            // server informs data server to start looking for connections
                            kill(c, SIGUSR1);
                            int addrlen = sizeof(data_address[req-1]);
                            if((data_server_sfd[req-1] = accept(data_socket[req-1], (struct sockaddr *)&data_address[req-1], (socklen_t*)&addrlen))<0){
                                perror("accept");
                                exit(EXIT_FAILURE);
                            }

                            //pthread_create(&read_threads[req-1], NULL, reading, (void *)req-1);
                        }
                        else{
                            char *path[] = {data_server_files[req-1], NULL};
                            execv(path[0], path);
                        }
                    }
                    
                    // add the nsfd to the list of nsfd for every data server
                    int it = 0;
                    while(nsfd_data[req-1][it] != 0){
                        it++;
                    }
                    nsfd_data[req-1][it] = nsfd[i];
                    // read from data server thread                    
                    write(nsfd[i], "Connection with data server established\n", 1024);
                }
            }
        }
    }
}