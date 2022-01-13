#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
int main(){


    char buf[2048];
    printf("Child wants input");
    scanf( "%s", buf);

    write(1,buf,2048) ;
    printf("Child sends %s\n", buf);
    printf("\n");
    
}