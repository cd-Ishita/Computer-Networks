#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
using namespace std;
int main(){
	int fd1[2], fd2[2];
	pipe(fd1);
	pipe(fd2);
	int c = fork();

	if(c > 0){
		close(fd1[0]);
		close(fd2[1]);
		//fd1 write fd2 read
		char buf[2048];
		
		cout<<"Process P has written: "<<buf<<endl;
		write(fd1[1], buf, 2048);

		close(fd1[1]);

		wait(NULL);

		char buf1[2048];
		read(fd2[0], buf1, 2048);
		cout<<"Process P has read: "<<buf1<<endl;
	}
	else{
		close(fd1[1]);
		close(fd2[0]);
		//fd1 read fd2 write

		char buf2[2048];
		read(fd1[0], buf2, 2048);
		cout<<"Process P' has read: "<<buf2<<endl;

		close(fd1[0]);

		char buf3[2048];
		cin>>buf3;
		cout<<"Process P' has written: "<<buf3<<endl;
		write(fd2[1], buf3, 2048);

		cout<<"Process P' is ending"<<endl;

	}
}