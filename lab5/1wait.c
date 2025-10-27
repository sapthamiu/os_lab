#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>

int main(){
    pid_t pid = fork();
    if(pid < 0){
        perror("fork failed");
        exit(1);
    }
    else if(pid == 0){
        sleep(3);
        printf("Hello from child\n");
    }
    else{
        printf("Hello from parent\n");
        wait(NULL);
        printf("Child finished\n");
    }
    return 0;
}