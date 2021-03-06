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

    int c = fork();
    if(c > 0){
        int fd = open("green", O_RDONLY);
        sleep(5);
        char buf[2048];
        read(fd, buf, 2048);

        printf("Parent read %s\n", buf);
    }
    else{
        char *args[] = {"./test", NULL};
        execv(args[0], args);
    }
}