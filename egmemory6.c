#include<stdio.h>
#include<stdlib.h>
#define ONE_K 1024
int main(){
	char* some_memory;
	int exit_code = 1;
	some_memory = (char*) malloc(ONE_K);
	if(some_memory != NULL){
		free(some_memory);
		printf("Memory allocated and freed again\n");
		exit_code = 0;
	}
	exit(exit_code);
}