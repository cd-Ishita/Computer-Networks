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

    int c = fork();
    if(c > 0){
        int fd = open("green", O_WRONLY);
        //sleep(5);
        printf("Parent wants input");
        char buf[2048];
        scanf("%s", buf);
        write(fd, buf, 2048);

        //printf("Parent read %s\n", buf);

        close(fd);
        unlink("green");
    }
    else{
        char *args[] = {"./test", NULL};
        execv(args[0], args);
    }

    
}