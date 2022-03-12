#include<stdio.h>
int main(){

    char buffer[2048];
    char buffer1[2048];
    int i=0;
    while(buffer[i] != '\0'){
        buffer1[i] = (char)((int)buffer[i] - 32);
        i++;
    }
    printf("%s", buffer1);
    recv(nsfd, buffer, 2048);
    send(nsfd, buffer1, 2048);
}