#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(){
	char* some_memory = (char*)0;
	printf("A read from null %s\n", some_memory);
	sprintf(some_memory, "A write to null\n");
	exit(0);
}