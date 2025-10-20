#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t resource1, resource2;

void *threadA(void *arg) {
    printf("Thread A: Trying to lock Resource 1\n");
    sem_wait(&resource1);    // acquire first resource
    sleep(5);                // simulate some work

    printf("Thread A: Trying to lock Resource 2\n");
    sem_wait(&resource2);    // try to acquire second resource
    printf("Thread A: Acquired both resources!\n");

    // critical section
    sem_post(&resource2);
    sem_post(&resource1);
    return NULL;
}

void *threadB(void *arg) {
    printf("Thread B: Trying to lock Resource 2\n");
    sem_wait(&resource2);    // acquire second resource first
    sleep(3);                // simulate some work

    printf("Thread B: Trying to lock Resource 1\n");
    sem_wait(&resource1);    // try to acquire first resource
    printf("Thread B: Acquired both resources!\n");

    // critical section
    sem_post(&resource1);
    sem_post(&resource2);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // initialize semaphores as binary (1)
    sem_init(&resource1, 0, 1);
    sem_init(&resource2, 0, 1);

    pthread_create(&t1, NULL, threadA, NULL);
    pthread_create(&t2, NULL, threadB, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&resource1);
    sem_destroy(&resource2);

    return 0;
}
