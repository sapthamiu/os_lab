#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
int main(){
	int fd[2], n;
	char buf[1024];
	char* data = "hello pipe";
	pipe(fd);
	write(fd[1], data, strlen(data));
	if((n = read(fd[0], buf, sizeof(buf))) >= 0){
		buf[n] = '\0';
		printf("Read %d bytes from the pipe: %s\n", n, buf);
	}
	else perror("read");
	exit(0);
}