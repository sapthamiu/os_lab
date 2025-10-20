#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "shm.h"

int main(){
    SHM_MSG* shm;
    int shmid = shmget(SHM_KEY, sizeof(shm), 0666| IPC_CREAT);
    shm = (SHM_MSG*)shmat(shmid, NULL, 0);
    shm->written = 0;
    while(1){
        char buf[MAX];
        printf("Enter your message: ");
        fgets(buf, MAX, stdin);
        if(strcmp(buf, "exit")==0){
            strcpy(shm->data, buf);
            break;
        }
        strcpy(shm->data, buf);
        sleep(1);
    }
    shmdt(shm);
    return 0;
}