#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
int main(){
    int pfd1[2], pfd2[2];
    pipe(pfd1);
    pipe(pfd2);
    int c = fork();

    if(c>0){
        close(pfd1[0]);
        close(pfd2[1]);

        printf("This is P1 process\n");
        printf("P1 wants input: \n");
        char buf[2048];
        scanf("%s", buf);

        printf("P1 is writing %s to pipe\n", buf);
        write(pfd1[1], buf, 2048);

        wait(NULL);

        char buf1[2048];
        read(pfd2[0], buf1, 2048);
        printf("P1 read %s from pipe\n", buf1);
    }
    else{
        close(pfd2[0]);
        close(pfd1[1]);

        dup2(pfd1[0], 0);
        dup2(pfd2[1], 1);

        close(pfd1[0]);
        close(pfd2[1]);
        char *args[] = {(char *)("./p2"), NULL};
        execv("./p2", args);
    }
}