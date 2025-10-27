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
        printf("Going into first program\n");
        execl("./1wait", "1wait", NULL);
    }
    else{
        wait(NULL);
        printf("Child finished");
    }
    return 0;
}