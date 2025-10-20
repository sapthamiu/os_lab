#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUF_SIZE 5

int buf[BUF_SIZE], f = -1, r = -1;
int produced = 0, consumed = 0;

sem_t mutex, full, empty, limit;

void *produce(void *arg) {
    for (int i = 0; i < 30; i++) {  // produce 30 items for demo
        sem_wait(&limit);     // ensure produced - consumed <= 10
        sem_wait(&empty);     // ensure buffer space available
        sem_wait(&mutex);

        r = (r + 1) % BUF_SIZE;
        buf[r] = i;
        produced++;
        printf("Produced item: %d (Produced=%d, Consumed=%d)\n", i, produced, consumed);

        sem_post(&mutex);
        sem_post(&full);
        sleep(1);
    }
    return NULL;
}

void *consume(void *arg) {
    for (int i = 0; i < 30; i++) {  // consume 30 items for demo
        sem_wait(&full);   // wait for available item
        sem_wait(&mutex);

        f = (f + 1) % BUF_SIZE;
        int item = buf[f];
        consumed++;
        printf("Consumed item: %d (Produced=%d, Consumed=%d)\n", item, produced, consumed);

        sem_post(&mutex);
        sem_post(&empty);
        sem_post(&limit);   // consumer frees up "limit gap"
        sleep(2);           // slower consumer for testing
    }
    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUF_SIZE);
    sem_init(&limit, 0, 10);   // ensures producer ≤ consumer + 10

    pthread_create(&tid1, NULL, produce, NULL);
    pthread_create(&tid2, NULL, consume, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&limit);

    return 0;
}
