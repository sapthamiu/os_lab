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
        printf("In child process:\tPID: %d\tPPID: %d\n", getpid(), getppid());
    }
    else{
        printf("In parent process:\tPID: %d\tPPID: %d\tCPID: %d\n", getpid(), getppid(), pid);
        wait(NULL);
        printf("Child finished");
    }
    return 0;
}