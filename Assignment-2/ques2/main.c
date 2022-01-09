//along with one, do exec("p3.exe") in parent and in p2:<do sleep(10000)>, in p3:<do sleep(5000)> and see which program<among p2 and p3> completes first with wait(&pid)

//The first code uses 2 children

//The second code, p3 is executed by parent
//p3 sleeps for 5 secs while p2 sleeps for 10, therefore parent finishes before child. Because parent thread data is replaced by p3, the wait() part is not visible as well


/*#include<stdio.h>
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

	int pid_1, pid_2, ppid;
	if(c > 0){
		
		c2 = fork();
		if(c2 > 0){
			ppid = getpid();
			printf("This is the parent process with pid%d:\n", ppid);
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
	//waitpid(pid_1, &status, WNOHANG);
	//waitpid(pid_2, &status, WNOHANG);
	printf("Child process with pid:%d finished first\n",waitpid(-1, &status, 0));
	wait(NULL);
	printf("Exit pid:%d\n", getpid());

}*/

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

	int pid_1, pid_2, ppid;
	if(c > 0){
		pid_2 = getpid();
		
		printf("This is parent with pid:%d\n", pid_2);
		char *args[] = {"./p3", NULL};
		execv(args[0], args);
		printf("Parent has finished executing p3.exe\n");
	}
	else{
		pid_1 = getpid();
		printf("This is the child 1 with pid:%d\n", pid_1);
		char *args[] = {"./p2", NULL};
		execv(args[0], args);
		printf("Child 1 has finished executing p2.exe\n");
	}
	int status;
	//waitpid(pid_1, &status, WNOHANG);
	//waitpid(pid_2, &status, WNOHANG);
	printf("Process with pid:%d has finished\n",getpid());
	wait(NULL);
	printf("Exit pid:%d\n", getpid());

}