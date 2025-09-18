#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

#define FIFONAME "/tmp/myfifo"

int main(){
	int res;
	int nums[4];
	printf("Consumer %d opening FIFO O_RDONLY\n", getpid());
	int pipe_fd = open(FIFONAME, O_RDONLY);// read only mode, waits until a producer opens the FIFO
	if(pipe_fd == -1){
		perror("open failed");
		exit(EXIT_FAILURE);
	}
	res = read(pipe_fd, nums, sizeof(nums));
	if(res == -1){
		perror("read failed");
		exit(EXIT_FAILURE);
	}
	printf("Consumer read %d bytes. \nNumbers are: ", res);
	for(int i = 0; i < 4; i++)
		printf("%d ", nums[i]);
	printf("\n");
	close(pipe_fd);
}