#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<limits.h>
#include<sys/types.h>
#include<sys/stat.h>

#define FIFONAME "/tmp/myfifo"
#define BUFSIZE PIPE_BUF
int main(){
	int res;
	char buf[BUFSIZE+1];
	int bytes_read = 0;
	memset(buf, '\0', sizeof(buf)); //for safety 
	printf("Process %d opening FIFO O_RDONLY\n", getpid());
	int pipe_fd = open(FIFONAME, O_RDONLY);// read only mode, waits until a producer opens the FIFO
	printf("Process %d, result %d\n", getpid(), pipe_fd);
	if(pipe_fd != 1){
		do{	//reads BUFSIZE chunks from FIFO until producer closes its write end
			res = read(pipe_fd, buf, BUFSIZE);
			bytes_read += res;
		} while(res > 0); //read returns 0 when producer closes
		close(pipe_fd);
	}
	else exit(EXIT_FAILURE);
	printf("Process %d finised, %d bytes read\n", getpid(), bytes_read);
	exit(EXIT_SUCCESS);
}