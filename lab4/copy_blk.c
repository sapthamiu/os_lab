#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
int main(){
	char blk[1024];
	int in, out;
	int nread;
	in = open("file.in", O_RDONLY);
	out = open("file.out", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	while((nread = read(in, blk, sizeof(blk))) > 0)
		write(out, blk, nread);
	exit(0);
}