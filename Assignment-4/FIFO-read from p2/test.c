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

    int fd = open("green", O_WRONLY);
    //sleep(5);
    char buf[2048];
    printf("Child asks input: ");
    scanf("%s", buf);
    write(fd, buf, 2048);
    printf("Child sent data to Parent");

}