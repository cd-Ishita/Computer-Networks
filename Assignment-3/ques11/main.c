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
	int pfd1[2], pfd2[2];
	pipe(pfd1);
	pipe(pfd2);

	int c;
	printf("This is the main process\n");

	c = fork();
	if(c > 0){
		printf("This is the parent process\n");
		close(pfd1[1]);
		close(pfd2[0]);

		char buf1[2048] = "Hello From Parent";
		write(pfd2[1], buf1, 2048);

		printf("Parent is sending %s to pipe\n", buf1);

		wait(NULL);

		char buf2[2048];
		read(pfd1[0], buf2, 2048);

		printf("Parent read %s from pipe\n", buf2);
	}
	else{
		close(pfd1[0]);
		close(pfd2[1]);

		dup2(pfd2[0], 0);
		dup2(pfd1[1], 1);

		close(pfd2[0]);
		close(pfd1[1]);

		char *args[] = {"./p2", NULL};
		execv(args[0], args);
		printf("Child has finished executing p2.exe\n");
	}
	wait(NULL);
	printf("Child process has ended, only parent is here\n");
}