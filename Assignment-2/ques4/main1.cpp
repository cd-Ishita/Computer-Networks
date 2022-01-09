//use 1 fd[2] and pipe() to 1-way communicate from parent to child.
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
	int fd[2];
	pipe(fd);

	int c = fork();
	if(c > 0){
		close(fd[1]);
		char buf[2048];
		int status;
		waitpid(c, &status, 0);
		read(fd[0], buf, 2048);
		cout<<buf;
		
	}
	else{
		close(fd[0]);
		char buf1[2048];
		cin>>buf1;
		write(fd[1], buf1, 2048);
		close(fd[1]);
	}
}