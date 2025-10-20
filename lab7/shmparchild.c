// parent_child_alphabet.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234

struct shared_data {
    char alphabet;
    int flag; // 0 = parent writes, 1 = child replies
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

    shm = (struct shared_data *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    shm->flag = 0; // Parent will write first

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) { 
        // Child process
        while (1) {
            if (shm->flag == 1) {
                char next = shm->alphabet + 1;
                if (shm->alphabet == 'z' || shm->alphabet == 'Z') {
                    printf("Child: End of alphabet reached.\n");
                    break;
                }
                shm->alphabet = next;
                shm->flag = 0; // reply done
            }
        }
    } else {
        // Parent process
        char ch;
        printf("Enter an alphabet: ");
        scanf(" %c", &ch);

        shm->alphabet = ch;
        shm->flag = 1; // tell child

        sleep(1); // give child time to respond

        if (shm->flag == 0) {
            printf("Parent received next alphabet: %c\n", shm->alphabet);
        }

        // Cleanup
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
