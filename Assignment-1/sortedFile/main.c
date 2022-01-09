#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){

	int fd = open("f1.txt", O_RDONLY);
	char buff[100];
	int last = read(fd, buff, 100);
	
	int array1[10];
	for(int i=0;i<last;i=i+2){
		array1[i/2] = (int)buff[i] - 48;
		array1[(i/2)+1] = '\0';
		printf("%d ", array1[i/2]);
	}

	close(fd);

	fd = open("f2.txt", O_RDONLY);
	last = read(fd, buff, 100);

	int array2[10];
	for(int i=0;i<last;i=i+2){
		array2[i/2] = (int)buff[i] - 48;
		array2[(i/2)+1] = '\0';
		printf("%d ", array2[i/2]);
	}
	close(fd);


	int mergedArray[20];
	int i=0,j=0,k=0;
	while(array1[i] != '\0'){
		if(array1[i] == '\0'){
			mergedArray[k] = array2[j];
			j++;
		}
		else if(array2[j] == '\0'){
			mergedArray[k] = array1[i];
			i++;
		}
		else if(array2[j] <= array1[i]){
			mergedArray[k] = array2[j];
			j++;
		}
		else if(array1[i] < array2[j]){
			mergedArray[k] = array1[i];
			i++;
		}
		k++;
	}
	mergedArray[k] = '\0';

	fd = open("f3.txt",  O_WRONLY);
	char res[200];
	i=0, j=0;
	for(i=0;i<k;i++){
		printf("%d", (mergedArray[i] + 48));
		res[j] = (char)(mergedArray[i] + 48);
		j++;
		res[j] = ' ';
		j++;
	}
	res[j] = '\0';
	write(fd, res, strlen(res));

}