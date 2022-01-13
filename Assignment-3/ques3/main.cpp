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

		printf("This is the parent\n");

		char buf1[2048] = "keyboard input to parent";

		write(pfd1[1], buf1, 2048);

		wait(NULL);

		char buf2[2048];
		read(pfd2[0], buf2, 2048);
		printf("Parent read %s from the child\n", buf2);
	}
	else{
		close(pfd1[1]);
		close(pfd2[0]);

		dup2(pfd1[0], 0);
		dup2(pfd2[1], 1);

		close(pfd1[0]);
		close(pfd2[1]);

		printf("This is the child\n");

		char *args[] = {(char *)("./p2"), NULL};
		execv(args[0], args);
		
	}
	return 0;
}