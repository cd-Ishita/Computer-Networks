#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>
int main(){

	int fd = open("f1.txt", O_RDWR);
	char buff[1000];
	int last = read(fd, buff, 1000);
	
	close(fd);
	bool capital = true;
	for(int i=0;i<last;i++){
		if(capital == true && ((buff[i] >= 'a' && buff[i] <= 'z') || (buff[i] >= 'A' && buff[i] <= 'Z'))){ 
			if(buff[i] >= 'a' && buff[i] <= 'z'){
				buff[i] = (char)((int)buff[i] - 32);
				capital = false;
			}
		}
		else if(buff[i] == ' '){
			capital = true;
		}
	}

	fd = open("f3.txt", O_WRONLY);
	write(fd, buff, strlen(buff));
	
}