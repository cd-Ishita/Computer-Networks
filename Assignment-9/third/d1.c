#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include<signal.h>
#define PORT 10000

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

	if (connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	printf("D1 Connected to Main server\n");
	int i = 0;
	printf("D1");
    while(1){
        char buf[2048];
		sprintf(buf, "%d", i);
        send(sfd, buf, strlen(buf), 0);
		sleep(15);
    }

	return 0;
}