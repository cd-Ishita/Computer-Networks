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

char* read_thread(int fd){
    char buf[2048];
    read(fd, buf, 2048);
    char* buf1;
    strcpy(buf1, buf);
    return buf1;
}

void write_thread(int fd, char buf[2048]){
    write(fd, buf, 2048);
}

int main(){
    int c;

    mkfifo("FIFO1", 0666);
    mkfifo("FIFO2", 0666);

    int fifo1 = open("FIFO1", O_WRONLY);
    int fifo2 = open("FIFO2", O_RDONLY);
    fflush(stdout);

    char buf[2048] = "I am performer and this is my performance\n";
    write_thread(fifo1, buf);

    fflush(stdout);
    write_thread(fifo1, "/d");

    char* buf1 = read_thread(fifo2);

    char buf2[2048] = "I am performer and this is my answer to judge's question\n";
    write_thread(fifo1, buf2);
    printf("4");
    
    
    printf("Enter any char to find score: \n");
    c = getchar();
    char *buf3 = read_thread(fifo2);
    printf("My score is: %s\n", buf3);
    printf("I am leaving now\n");
}