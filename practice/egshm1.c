//We will write a pair of programs shm1.c and shm2.c. 
//The first will create a shared memory segment and display 
//any data that is written into it. The second will attach into
//an existing shared memory segment and enters data into the 
//shared memory segment.
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include "shm.h"

int main(){
    SHM_MSG *shm;
    int shmid = shmget(SHM_KEY, sizeof(shm), 0666|IPC_CREAT);
    if(shmid == -1){
        perror("shm failed");
        exit(1);
    }
    shm = (SHM_MSG*) shmat(shmid, NULL, 0);
    if(shm == (void*)-1){
        perror("shmat failed");
        exit(1);
    }
    while(1){
        if(shm->written == 1){
            if(strncmp(shm->data, "end", 3) == 0){
                printf("Terminating");
                break;
            }
            printf("Read: %s\n", shm->data);
            shm->data[0] = '\0';
            shm->written = 0;
        }
    }
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
