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

    int fd= 0;

    struct pollfd fds[1];
    int timeout;

    while(1){
        fds[0].fd = fd;
        fds[0].events = 0; //just to clear any previous events
        fds[0].events |= POLLIN;

        timeout = 5000; //in milliseconds

        int pret = poll(fds, 1, timeout);

        if(pret == 0){
            printf("No input so timeout\n");
        }
        else{
            char buf[2048];
            int i = read(fd, buf, 2048);
            printf("buf = %s", buf);
            if(!i){
                printf("We got input this: %s\n", buf);
            }
            
        }
    }

}