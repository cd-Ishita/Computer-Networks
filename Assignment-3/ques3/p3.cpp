#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
int main(){

	char buf1[2048];
	read(0, buf1, 2048);

	printf("Grandchild read %s from pipe\n", buf1);

	char buf2[2048] = "Hello from grandchild";
	write(1, buf2, 2048);

	return 0;
}