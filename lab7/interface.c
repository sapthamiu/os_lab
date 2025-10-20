// interface.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 1234
#define MAX_SIZE 256

struct shared_data {
    int ready;              // 0 = no new message, 1 = new message available
    char message[MAX_SIZE]; // message buffer
};

int main() {
    int shmid;
    struct shared_data *shm;

    // Create shared memory
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

    shm->ready = 0; // initialize

    while (1) {
        char buffer[MAX_SIZE];
        printf("Enter your message: ");
        fflush(stdout);

        if (fgets(buffer, MAX_SIZE, stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0'; // remove newline

        // Exit condition
        if (strcmp(buffer, "exit") == 0) {
            strcpy(shm->message, "exit");
            shm->ready = 1; 
            break;
        }

        // Write to shared memory
        strcpy(shm->message, buffer);
        shm->ready = 1; // tell display
        sleep(1);       // allow display to process
    }

    shmdt(shm);
    return 0;
}
