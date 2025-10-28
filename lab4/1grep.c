#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
int main(int argc, char* argv[]){
	if(argc != 3){
		write(2, "Usage: ./1grep <word> <filename>\n", 32);
		exit(1);
	}
	char block[1024], line[2048];
	int in, lc=0, i, index = 0;
	ssize_t nread;
	char* word = argv[1];
	char* filename = argv[2];
	in = open(filename, O_RDONLY);
	while((nread = read(in, block, sizeof(block))) > 0){
		for(i = 0; i < nread; i++){
			line[index++] = block[i];
			if(block[i] == '\n'){
				line[index] = '\0';
				if(strstr(line, word) != NULL) 
					write(1, line, index);
				index = 0;
			}
		}
	}
	close(in);
	if(index > 0){
		line[index] = '\0';
		if(strstr(line, word) != NULL)
			write(1, line, index);
	}

}