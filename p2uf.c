#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>


int main(int argc, char *argv[]){
	int maxInd = -1;
	int fd = open("sendThroughFifo", O_RDONLY);
	if(fd<0){
		printf("Failed To Open the file\n");
		return 1;
	}
	if(mkfifo("ackThroughFifo", 0777)==-1){
		printf("Failed To generate the Fifo File\n");
		return 1;
	}
	int count = 0;
	for(int a = 0; a<10; a++){
		char *buffer = (char*)calloc(26, sizeof(char));
		if(read(fd, buffer, 26)<0){
			printf("Failed to read the file\n");
			return 1;
		}
		int i = 0;
		
		printf("---String Set No:%d---\n",k+1);
		while(i<26){
			if((i+1)%5 != 0 || i==0){
				printf("%c", buffer[i]);
			}else{
				if(maxInd<(int)buffer[i]){
					maxInd = (int)buffer[i]-65;
				}
				printf(", ID_no_is :%d\n", maxInd);
			}
			i++;
		}
		printf("-------------------------------------------------\n");

		int fd2 = open("ackThroughFifo", O_WRONLY);
		if(fd2<0){
			printf("Failed to open the file\n");
			return 1;
		}
		if(write(fd2, &maxInd, sizeof(int))==-1){
			printf("Failed To Write into the file\n");
			return 1;
		}
	}

	sleep(1);
	return 0;
}
