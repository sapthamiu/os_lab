#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define MB 1024*1024
int main(){
	char* some_memory = (char*) malloc((int)MB);
	int exit_code = EXIT_FAILURE;
	if(some_memory != NULL){
		sprintf(some_memory, "Hello World\n");
		printf("%s", some_memory);
		exit_code = EXIT_SUCCESS;
	} exit(exit_code);
}