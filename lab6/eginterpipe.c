#include<sys/wait.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int main(int argc, char* argv[]){
	int pfd[2];
	char buf[1024];
	assert(argc == 2);
	if(pipe(pfd) == -1){
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid_t cpid = fork();
	if(cpid == -1){
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(cpid == 0){
		//child reads
		close(pfd[1]); //close write end
		int n = read(pfd[0], buf, sizeof(buf));
		if(n > 0)
			write(STDOUT_FILENO, buf, n);
		write(STDOUT_FILENO, "\nDone\n", 6);
		close(pfd[0]);
		exit(EXIT_SUCCESS);
	}
	else{
		//parent writes
		close(pfd[0]); //close read end
		write(pfd[1], argv[1], strlen(argv[1]));
		close(pfd[1]);
		wait(NULL);
		exit(EXIT_SUCCESS);
	}
}