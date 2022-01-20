#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg{
    long type ;
    //int sz ;
    char buf[2048];
};
// struct msg msg_init(int t){
//     struct msg msg_new ;
//     msg_new.type=t ;
//     return msg_new ;
// }
int main(){
    key_t key=ftok("message",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
    

    struct msg new_message ;
    msgrcv(msqid,&new_message,2048,1,0);

    printf("Message type received: %ld\n", new_message.type);
    printf("Message content: %s\n", new_message.buf);
    msgctl(msqid, IPC_STAT, NULL);
	msgctl(msqid, IPC_RMID, NULL);
    return 0 ;
 }