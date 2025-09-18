#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

#define FIFONAME "/tmp/myfifo" //path of the FIFO file

int main(){
	int res;
	int nums[4] = {10, 20, 30, 40};

	if(access(FIFONAME, F_OK) == -1){ //check if FIFO already exists
		res = mkfifo(FIFONAME, 0777); //create pipe if not existing
		if(res != 0){
			fprintf(stderr, "Could not create fifo %s\n", FIFONAME);
			exit(EXIT_FAILURE);
		}
	}
	printf("Producer %d opening FIFO O_WRONLY\n", getpid());
	int pipe_fd = open(FIFONAME, O_WRONLY); //write only mode, waits until a consumer opens the FIFO
	if(pipe_fd == -1){
		perror("open failed");
		exit(EXIT_FAILURE);
	}

	res = write(pipe_fd, nums, sizeof(nums)); 
	if(res == -1){
		perror("Write failed");
		exit(EXIT_FAILURE);
	}
	printf("Producer wrote %d bytes to FIFO\n", res);
	close(pipe_fd);
	exit(EXIT_SUCCESS);
}