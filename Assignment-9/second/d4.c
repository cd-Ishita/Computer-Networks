#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include<signal.h>
#define PORT 10003

int sfd = 0;
int main(){
    int valread;
	struct sockaddr_in serv_addr;
	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}

    printf("D4 connection established\n");
	int i = 0;
    //while(1){
        send(sfd, "D4 says hello!", 2048, 0);
		//sleep(5);
    //}

	return 0;
}