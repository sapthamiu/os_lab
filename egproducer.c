#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<limits.h>
#include<sys/types.h>
#include<sys/stat.h>

#define FIFONAME "/tmp/myfifo" //path of the FIFO file
#define BUFSIZE PIPE_BUF //max write size defined by the system, usually 4096 bytes
#define TEN_MEG (1024*1024*10) //total data to send, 10MB

int main(){
	int res;
	int bytes_sent = 0;
	char buf[BUFSIZE+1]; //ot iitialised wit data, but doesnt matter since we measure throughput 
	if(access(FIFONAME, F_OK) == -1){ //check if FIFO already exists
		res = mkfifo(FIFONAME, 0777); //create pipe if not existing
		if(res != 0){
			fprintf(stderr, "Could not create fifo %s\n", FIFONAME);
			exit(EXIT_FAILURE);
		}
	}
	printf("Process %d opening FIFO O_WRONLY\n", getpid());
	int pipe_fd = open(FIFONAME, O_WRONLY); //write only mode, waits until a consumer opens the FIFO
	printf("Process %d, result %d\n", getpid(), pipe_fd);
	if(pipe_fd != -1){
		while(bytes_sent < TEN_MEG){ // writes chunks of BUFSIZE until 10MB is written
			res = write(pipe_fd, buf, BUFSIZE); 
			if(res == -1){
				fprintf(stderr, "Write error on pipe\n");
				exit(EXIT_FAILURE);
			}
			bytes_sent += res;
		}
		close(pipe_fd);
	}
	else exit(EXIT_FAILURE);
	printf("Process %d finished \n", getpid());
	exit(EXIT_SUCCESS);
}