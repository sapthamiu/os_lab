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
        printf("Hello from child\n");
        printf("PPID: %d Going to sleep\n", getppid());
        sleep(20);
        printf("PPID: %d\n", getppid());
    }
    else{
        printf("Hello from parent\n");
        exit(0);
    }
    return 0;
}