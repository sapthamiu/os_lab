// display.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 1234
#define MAX_SIZE 256

struct shared_data {
    int ready;
    char message[MAX_SIZE];
};

int main() {
    int shmid;
    struct shared_data *shm;

    // Access shared memory
    shmid = shmget(SHM_KEY, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    // Attach
    shm = (struct shared_data *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    while (1) {
        if (shm->ready == 1) {
            if (strcmp(shm->message, "exit") == 0) {
                printf("Display: Exit command received. Terminating.\n");
                break;
            }

            printf("Display: %s\n", shm->message);

            // Clear message & reset flag
            shm->message[0] = '\0';
            shm->ready = 0;
        }
        usleep(100000); // small delay to avoid busy-waiting
    }

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL); // cleanup shared memory
    return 0;
}
