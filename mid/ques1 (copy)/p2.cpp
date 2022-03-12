#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<poll.h>
#include<semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <math.h>   
#include<pthread.h>  
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/select.h>
#include<iostream>
using namespace std;
int main(){
    cout<<"helloooo";
    
    mkfifo("FIFO1", 0666);
    mkfifo("FIFO2", 0666);

    int fifo1 = open("FIFO1", O_WRONLY);
    int fifo2 = open("FIFO2", O_RDONLY);

    printf("0");
    char buf[2048] = "I am performer and this is my performance\n";
    write(fifo1, buf, 2048);
    printf("1");
    write(fifo1, "/d", 2048);
    printf("2");
    char buf1[2048];
    read(fifo2, buf1, 2048);
    printf("3");
    char buf2[2048] = "I am performer and this is my answer to judge's question\n";
    write(fifo1, buf2, 2048);
    printf("4");
    char buf3[2048];
    read(fifo2, buf3, 2048);
    printf("My score is: %s\n", buf3);
    printf("I am leaving now\n");
}