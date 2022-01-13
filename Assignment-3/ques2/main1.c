#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
int main(){
	int pfd1[2], pfd2[2];
	pipe(pfd1);
	pipe(pfd2);

	int c = fork();
	if(c > 0){
		close(pfd1[0]);
		close(pfd2[1]);
		//pfd1 write pfd2 read

		char buf1[2048]="hello from parent";
		printf("Parent wants input\n");
		printf("Parent process is writing %s to the pipe\n", buf1);
		write(pfd1[1], buf1, 2048);

		wait(NULL);
		char buf[2048];
		read(pfd2[0], buf, 2048);
		printf("Parent process read %s from the pipe\n", buf);
	}
	else if( c == 0){
		printf("Child process begin\n");
		dup2(pfd1[0], 0);
		dup2(pfd2[1], 1);
		close(pfd1[0]);
		close(pfd1[1]);
		close(pfd2[0]);
		close(pfd2[1]);
		// const char **args = new const char*[2] ;
		// const char *programname = "./p2";
		// args[0] = programname ;
		// args[1] = NULL ;
		// execv(programname, (char **)args);

		char *args[] = {"./p1", NULL};
		execv(args[0], args);
	}
	else{
		
	}
}