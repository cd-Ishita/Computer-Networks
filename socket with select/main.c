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

struct thread_args{
    int id;
    int nsfd;
};
#define MAX 4
int cnt=0;
int A[MAX];

// Thread function, every thread for every client uses this
void* func1(void * arg){
    struct thread_args* args = (struct thread_args*)arg;
    int id = args->id;
    int nsfd = args->nsfd;
    
    char buf[2048] = {0};
    read(nsfd, buf, 2048);
    printf("File %d has sent %s\n", id, buf);
    cnt--;
    A[id] = 0;
    pthread_exit(NULL);
    
}

int main(){
    // Number of sockets
    int num = 4;
    int sfd[num];
    int port_num[num];
    struct sockaddr_in address[num];

    //configure to take this info from config file
    port_num[0] = 6060;
    port_num[1] = 7070;
    port_num[2] = 9090;
    port_num[3] = 8080;

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

    // -------------------- select ----------------

    pthread_t tid[num];
    int cnt = 0;
    fd_set rfds;
    int addrlen = sizeof(address); 
    
    memset(A, 0, num*sizeof(A[0]));

    while(1){
        int cnt = 0;
		FD_ZERO(&rfds);
		FD_SET(sfd[0],&rfds);
		FD_SET(sfd[1],&rfds);
		FD_SET(sfd[2],&rfds);
        FD_SET(sfd[3], &rfds);

		int ma=-1;
		for(int i=0;i<4;i++){
			if(ma<sfd[i])
	    		ma=sfd[i];
	    }
		int count = select(ma+1,&rfds,NULL,NULL,NULL);
		//After the select call, rfds set has been modified to contain those sfd that are ready to read
		
        //Check if all A are being used rn
        // cnt suggests can we take another connection? or are all being used rn
        for(int it=0;it<num;it++){
            if(A[it] == 1){
                cnt++;
            }
        }

        if(count>0){
			int free=0;
			for(int i=0;i<num;i++){
				char ch[120];
				if(cnt < num){	
                    // Check is sfd[i] is a part of rfds
                    if(FD_ISSET(sfd[i],&rfds)){	
						int nsfd;
   						if ((nsfd = accept(sfd[i], (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) { 
                            perror("accept"); 
                            exit(EXIT_FAILURE); 
    					} 
    					else{
    						printf("\ncount: %d\n", cnt);
                            struct thread_args arg;
                            arg.id = i;
                            arg.nsfd = nsfd;
                            pthread_create(&tid[i],NULL,func1,(void *)&arg);
    						
                            for(int it=0;it<num;it++){
                                if(A[it] == 0){
                                    A[it] = 1;
                                    break;
                                }
                            }
    					}
					}
					else{
                        free++;
                    }
				}
				else{
					printf("Platforms are full, sleeping now\n");
					sleep(1);
                    for(int i=0;i<num;i++){
                        pthread_join(tid[i], NULL);
                    }
				}	
			}
			if(free == num){
                // It comes to this if none of the sockets received a connection
			    printf("Timeout reached\n");
            }
		}
	}
}