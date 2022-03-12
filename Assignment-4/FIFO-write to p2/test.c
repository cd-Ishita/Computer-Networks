//compile with -lpthread -lrt
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
    mkfifo("green", 0666);

    int fd = open("green", O_RDWR);
    sleep(5);
    char buf[2048];
    read(fd, buf, 2048);
    printf("Child read %s", buf);

    printf("Child wants input: \n");
    scanf("%s", buf);
    write(fd, buf, 2048);
    printf("Child has written to parent\n");
    close(fd);
    unlink("green");
}