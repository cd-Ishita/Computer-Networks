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
    mkfifo("FIFO3", 0666);
    int fd = open("FIFO3", O_WRONLY);

    while(1){
        printf("P3 wants input: ");
        char buf[2048] = {0};
        scanf("%s", buf);
        char buf1[2048] = "P3: ";
        strcat(buf1,buf);
        write(fd, buf1, 2048);
    }
}