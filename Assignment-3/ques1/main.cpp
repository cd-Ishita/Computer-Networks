#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
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

		char buf1[2048];
		printf("Parent wants input\n");
		fgets(buf1, 2048, stdin);
	
		while(buf1 != NULL){
			write(pfd1[1], buf1, 2048);

			wait(NULL);

			char buf2[2048];
			read(pfd2[0], buf2, 2048);
			printf("Parent read %s from the child\n", buf2);
			printf("Parent wants input\n");
			fgets(buf1, 2048, stdin);
		}
	}
	else{
		close(pfd1[1]);
		close(pfd2[0]);

		printf("This is the child\n");
		char buf1[2048];
		printf("Child read %s from parent\n", buf1);

		//char buf2[2048] = "Parent Hi";
		char buf2[2048];
		cout<<"Child wants input";
		cin>>buf2;

		while(buf2 != NULL){
			cout<<"Child is sending "<<buf2<<" to parent"<<endl;
			write(pfd2[1], buf2, 2048);

			char buf1[2048];
			read(pfd1[0], buf1, 2048);
			cout<<"Child read "<<buf1<<" from parent"<<endl;

			char buf2[2048];
			cout<<"Child wants input";
			cin>>buf2;
			
		}
	}
	return 0;
}