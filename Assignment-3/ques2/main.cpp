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
	int pfd1[2], pfd2[2];
	pipe(pfd1);
	pipe(pfd2);

	int c = fork();
	if(c > 0){
		close(pfd1[0]);
		close(pfd2[1]);
		//pfd1 write pfd2 read

		char buf1[2048];
		cout<<"Parent wants input"<<endl;
		cin>>buf1;
		cout<<"Parent process is writing "<<buf1<<" to the pipe"<<endl;
		write(pfd1[1], buf1, 2048);

		wait(NULL);
		char buf[2048];
		read(pfd2[0], buf, 2048);
		cout<<"Parent process read "<<buf<<" from the pipe"<<endl;
	}
	else if( c == 0){
		cout<<"Child process begin"<<endl;
		dup2(pfd1[0], 0);
		dup2(pfd2[1], 1);
		close(pfd1[0]);
		close(pfd1[1]);
		close(pfd2[0]);
		close(pfd2[1]);
		//char *path = (char*)"./p2";
		char *args[] = {"./p2", NULL};
		
		//const char **args = new const char*[2] ;
		//const char *programname = "./p2";
		//args[0] = programname ;
		//args[1] = NULL ;
		execv(args[0], args);
		//execv(programname, (char **)args);
	}
	else{
		cout<<"error";
	}
}