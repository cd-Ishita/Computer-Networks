#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<unistd.h>

struct msg{
    long type ;
    //int sz ;
    char buf[2048];
};

int main(){
    key_t key=ftok("message",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
    
        struct msg new_message ;
        new_message.type = (long int)getpid();
        
        printf("Message to send: \n");
        fgets(new_message.buf, 2048, stdin);
        msgsnd(msqid,&new_message,2048,0);
    
    msgctl(msqid, IPC_RMID, NULL) ;

    return 0 ;
 }