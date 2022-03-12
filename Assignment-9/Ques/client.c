#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8081
int main(){
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	printf("1");
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
		return -1;
	}
    printf("2");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	printf("3");
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
    printf("4");
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
    printf("connected");
    char buf[2048] = "D1";
	send(sock , buf, 2048, 0);
	
	return 0;
}