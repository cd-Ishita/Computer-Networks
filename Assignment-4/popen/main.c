#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
int main(){
    FILE* fd = popen("./p2", "r");
    //sleep(5);

    char buf[2048];
    read(fileno(fd), buf, 2048);

    printf("Parent read %s", buf);
}