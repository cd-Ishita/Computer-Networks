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
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<poll.h>
#include<semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <math.h>
struct thread_args{
    int nsfd;
    int id;
};


int main(){
    mkfifo("FIFO1", 0666);
    mkfifo("FIFO2", 0666);
    mkfifo("FIFO3", 0666);

    int fd[3];
    fd[0] = open("FIFO1", O_RDONLY);
    fd[1] = open("FIFO2", O_RDONLY);
    fd[2] = open("FIFO3", O_RDONLY);

    // ----------------------------------------- //
    // -------- POLLING -------------//
    struct pollfd fds[3];
    memset(fds, 0 , sizeof(fds));

    for(int i=0;i<3;i++){
        fds[i].fd = fd[i];
        fds[i].events |= POLLIN;
    }
    int timeout = (3 * 60 * 1000);
    int connections = 0;
    while(connections != 3){
        int rc = poll(fds, 3, timeout);
        if(rc < 0){
            perror("poll failed");
            break;
        }
        if (rc == 0){
            printf("  poll() timed out.  End program.\n");
            break;
        }

        for (int i=0;i<3;i++){
            if(fds[i].revents == 0)
                continue;

            if(fds[i].revents & POLLIN){
                connections++;
                char buf[2048];
                read(fds[1].fd, buf, 2048);
                printf("buf = %s", buf);

            }
        }
    }
}