#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main() {
	int c;
	printf("This is the main process\n");

	c = fork();
	if(c>0){
		printf("This is parent process\n");
	}
	else{
		printf("This is child process\n");
	}

}