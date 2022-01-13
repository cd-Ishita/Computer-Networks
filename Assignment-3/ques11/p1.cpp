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
	cout<<"byeeeee";
	char buf1[2048] = "hello from child";

	write(1, buf1, 2048);
	cout<<"Child is writing "<<buf1<<" to pipe";

	char buf2[2048];
	read(0, buf2, 2048);
	cout<<"Child has read "<<buf2<<" from pipe";

	return 0;
}