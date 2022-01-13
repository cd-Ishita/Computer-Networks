#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
using namespace std;
int main() {
	cout<<"here";
	char buff[2048];
	read(0, buff, 2048);
	cout<<"Child process read "<<buff<<" from the pipe"<<endl;

	char buff1[2048];
	cout<<"Child wants input"<<endl;
	cin>>buff1;
	cout<<"Child process is writing "<<buff1<<" to the pipe"<<endl;
	write(1, buff1, 2048);

}