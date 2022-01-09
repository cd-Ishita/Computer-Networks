//write code in parent such that it executes a certain part of code after termination of a particular program<p2 or p3>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
int main(){
	int c, c2;
	printf("This is the main process\n");

	c = fork();

	pid_t pid_1, pid_2, ppid;
	if(c > 0){
		
		c2 = fork();
		if(c2 > 0){
			ppid = getpid();
			printf("This is the parent process with pid:%d\n", ppid);
		}
		else{
			pid_2 = getpid();
			
			printf("This is child 2 with pid:%d\n", pid_2);
			char *args[] = {"./p3", NULL};
			execv(args[0], args);
			printf("Child 2 has finished executing p3.exe\n");
		}
	}
	else{
		pid_1 = getpid();
		printf("This is the child 1 with pid:%d\n", pid_1);
		char *args[] = {"./p2", NULL};
		execv(args[0], args);
		printf("Child 1 has finished executing p2.exe\n");
	}

	int status;
	pid_t first = waitpid(-1, &status, 0);
	if(first == pid_1){
		printf("Child with pid %d has ended, this is parent", pid_1);
	}
	else if(first == pid_2){
		printf("Child with pid %d has ended, this is parent", pid_2);
	}
	else{
		printf("%d, %d", first, pid_2);
	}

	wait(NULL);
	
	printf("Exit pid:%d\n", getpid());

}