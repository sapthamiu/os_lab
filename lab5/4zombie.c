#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(){
    pid_t pid = fork();
    if(pid < 0){
        perror("fork failed");
        exit(1);
    }
    else if(pid == 0){
        printf("In the child process\n");
        exit(0);
    }
    else{
        printf("In the parent process\n");
        sleep(20);
        printf("Parent finishing\n");
    }
}