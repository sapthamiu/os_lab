#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
int* fibarr;
int n;
void* gen_fibo(void* param){
	if(n >= 1) fibarr[0] = 0;
	if(n >= 2) fibarr[1] = 1;
	for(int i = 2; i < n; i++)
		fibarr[i] = fibarr[i-1] + fibarr[i-2];
	return NULL;
}
int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Usage: %s <no. of terms>\n", argv[0]);
		return 1;
	}
	n = atoi(argv[1]);
	fibarr = (int*)malloc(n * sizeof(int));
	if(fibarr == NULL){
		printf("memory allocation failed\n");
		return 1;
	}
	pthread_t fibothread;
	pthread_create(&fibothread, NULL, gen_fibo, NULL);
	pthread_join(fibothread, NULL);
	printf("Fibonacci series: \n");
	for(int i = 0; i < n; i++)
		printf("%d ", fibarr[i]);
	printf("\n");
	return 0;
}