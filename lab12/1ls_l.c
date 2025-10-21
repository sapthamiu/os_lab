#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<string.h>
#include<unistd.h>

void print_perms(mode_t mode){
	char perms[11];

	//file type
	if(S_ISREG(mode)) perms[0] = '-';
	else if(S_ISDIR(mode)) perms[0] = 'd';
	else if(S_ISLNK(mode)) perms[0] = 'l';
	else if(S_ISCHR(mode)) perms[0] = 'c';
	else if(S_ISBLK(mode)) perms[0] = 'b';
	else if(S_ISFIFO(mode)) perms[0] = 'p';
	else if(S_ISSOCK(mode)) perms[0] = 's';
	else perms[0] = '?';

	//user
	perms[1] = (mode & S_IRUSR) ? 'r' : '-';
	perms[2] = (mode & S_IWUSR) ? 'w' : '-';
	perms[3] = (mode & S_IXUSR) ? 'x' : '-';
	//group
	perms[4] = (mode & S_IRGRP) ? 'r' : '-';
	perms[5] = (mode & S_IWGRP) ? 'w' : '-';
	perms[6] = (mode & S_IXGRP) ? 'x' : '-';
	//others
	perms[7] = (mode & S_IROTH) ? 'r' : '-';
	perms[8] = (mode & S_IWOTH) ? 'w' : '-';
	perms[9] = (mode & S_IXOTH) ? 'x' : '-';

	perms[10] = '\0';
	printf("%s", perms);
}

int main(){
	DIR* dir;
	struct dirent* entry;
	struct stat statbuf;
	long totblocks = 0;

	if(!(dir = opendir("."))){
		perror("opendir");
		return 1;
	}
	while((entry = readdir(dir))){
		if(strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0) continue;
		if(stat(entry->d_name, &statbuf) == -1){
			perror("stat");
			continue;
		}
		//find total blocks 
		totblocks += statbuf.st_blocks;
	}
	printf("total %1d\n", totblocks/2);

	//rewind directory to start printing
	rewinddir(dir);

	while((entry = readdir(dir))){
		if(strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0) continue;
		if(stat(entry->d_name, &statbuf) == -1){
			perror("stat");
			continue;
		}
		print_perms(statbuf.st_mode); //permissions

		printf("%2lu ", statbuf.st_nlink); //no. of hard links

		struct passwd *pw = getpwuid(statbuf.st_uid);
		printf("%-8s ", pw ? pw->pw_name : "unknown"); //owner name

		struct group *gr = getgrgid(statbuf.st_gid);
		printf("%-8s ", gr ? gr->gr_name : "unknown"); //group name

		printf("%6ld ", statbuf.st_size); //file size

		//last modification time
		char timebuf[80];
		strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&statbuf.st_mtime));
		printf("%s ", timebuf);

		printf("%s\n", entry->d_name);
	}
	closedir(dir);
	return 0;
}

//Output: 
/*
V_CSE_A1@debianpc-02:~/Desktop/230905090/OS/Lab12$ ./ls_l
total 44
-rwxr-xr-x 1 V_CSE_A1 V_CSE_A1  16352 Oct 21 09:58 a.out
-rw-r--r-- 1 V_CSE_A1 V_CSE_A1   2264 Oct 21 10:38 1ls-l.c
-rw-r--r-- 1 V_CSE_A1 V_CSE_A1    819 Oct 21 09:58 egscan.c
-rwxr-xr-x 1 V_CSE_A1 V_CSE_A1  16624 Oct 21 10:38 ls_l
*/