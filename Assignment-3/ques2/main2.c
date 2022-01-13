//do exec("p2.exe") in child
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
int main(){
	int c;
	printf("This is the main process\n");

	c = fork();
	if(c > 0){
		printf("This is the parent process\n");
	}
	else{
		printf("This is the child process\n");
		char *args[] = {"./p2", NULL};
		execv(args[0], args);
		printf("Child has finished executing p2.exe\n");
	}
	wait(NULL);
	printf("Child process has ended, only parent is here\n");
}