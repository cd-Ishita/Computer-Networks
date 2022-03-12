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
    mkfifo("FIFO4", 0666);
    int fd = open("FIFO4", O_WRONLY);

    while(1){
        printf("P4 wants input: ");
        char buf[2048] = {0};
        scanf("%s", buf);
        char buf1[2048] = "P4: ";
        strcat(buf1,buf);
        write(fd, buf1, 2048);
    }
}