#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
int main(){
    printf("In P2 process\n");

    char buf[2048];
    read(0, buf, 2048);
    printf("P2 reads %s from pipe\n", buf);

    printf("P2 wants input: \n");
    char buf1[2048];
    scanf("%s", buf1);
    printf("P2 is writing %s to pipe\n", buf1);
    write(1, buf1, 2048);
}