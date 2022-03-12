#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<iostream>
using namespace std;
int main(){
	char buf1[2048] = "hello from child";

	write(1, buf1, 2048);
	printf("Child is writing %s to pipe", buf1);

	char buf2[2048];
	read(0, buf2, 2048);
	printf("Child has read %s from pipe", buf2);

	return 0;
}