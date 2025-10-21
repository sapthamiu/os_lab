#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>

void list_files(char* dir, int depth){
	DIR *dp;
	struct dirent* entry;
	struct stat statbuf;
	char path[1024];

	if(!(dp = opendir(dir))){
		perror("opendir()");
		return;
	}
	while(entry = readdir(dp)){
		if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;
		snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name); //full path
		//get file info
		if(stat(path, &statbuf) == -1){
			perror("stat");
			continue;
		}
		//indentation
		for(int i = 0; i < depth; i++)
			printf(" ");

		printf("%s", entry->d_name);
		if(S_ISDIR(statbuf.st_mode)){ //directory
			printf("/\n");
			list_files(path, depth+1);
		}
		else printf("\n"); //file
	} closedir(dp);
}
int main(){
	char cwd[1024];
	if(!getcwd(cwd, sizeof(cwd))){
		perror("getcwd");
		return 1;
	}
	printf("Listing files in directory: %s\n", cwd);
	list_files(cwd, 0);
	return 0;
}