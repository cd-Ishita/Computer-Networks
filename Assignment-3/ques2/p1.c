#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>

int main() {
	printf("here");
	char buff[2048];
	read(0, buff, 2048);
	printf("Child process read %s from the pipe\n", buff);

	char buff1[2048] = "hello from child";
	printf("Child wants input\n");
	printf("Child process is writing %s to the pipe\n", buff1);
	write(1, buff1, 2048);

}