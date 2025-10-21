#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>

void printdir(char* dir, int depth){
	DIR *dp;
	struct dirent* entry;
	struct stat statbuf;
	if(!(dp = opendir(dir))){
		perror("opendir()");
		return;
	}
	chdir(dir);
	while(entry = readdir(dp)){
		lstat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode)){
			if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;
			printf("%*s%s\n", depth, "", entry->d_name);
			printdir(entry->d_name, depth+4);
		}
		else printf("%*s%s\n", depth, "", entry->d_name);
	}
	chdir("..");
	closedir(dp);
}
int main(int argc, char* argv[]){
	char* topdir;
	if(argc >= 2) topdir = argv[1];
	else topdir = ".";
	printf("Directory scan of %s:\n", topdir);
	printdir(topdir, 0);
	return 0;
}