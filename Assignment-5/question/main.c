#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<poll.h>
int main(){
    mkfifo("FIFO2", 0666);
    mkfifo("FIFO3", 0666);
    mkfifo("FIFO4", 0666);

    int fifo[3];
    fifo[0] = open("FIFO2", O_RDONLY);
    fifo[1] = open("FIFO3", O_RDONLY);
    fifo[2] = open("FIFO4", O_RDONLY);

    int num = 3;
    struct pollfd fds[num];
    int nsfd[num];
    memset(fds, 0 , sizeof(fds));

    for(int i=0;i<num;i++){
        fds[i].fd = fifo[i];
        fds[i].events |= POLLIN;
        fds[i].revents = 0;
    }
    int timeout = (3 * 60 * 1000);

    while(1){
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

            char buffer[2048] = {0};
            if((fds[i].revents & POLLIN) && read(fifo[i], buffer, 2048) != 0){
                printf("Message from process %d is %s\n", i+2, buffer);
                fds[i].revents = 0;
            }
        }
    }    
}