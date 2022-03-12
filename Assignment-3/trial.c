#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
int main(){
    int c = fork();
    if(c > 0){
        printf("This is parent process\n");
    }
    else{
        printf("This is child process\n");
        sleep(5);
        return 0;
    }
    wait(NULL);
    printf("Waiting for it to end\n");
}