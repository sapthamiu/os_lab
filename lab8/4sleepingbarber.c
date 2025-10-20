#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CHAIRS 5        // number of waiting chairs
#define CUSTOMERS 15    // total number of customers

sem_t customers_sem;   // counts waiting customers
sem_t barber_sem;      // signals barber to start cutting
sem_t mutex;           // protect access to waiting
int waiting = 0;       // number of waiting customers

void *barber(void *arg) {
    while (1) {
        sem_wait(&customers_sem);  // wait for a customer
        sem_wait(&mutex);
        waiting--;                  // one customer leaves waiting
        printf("Barber starts cutting hair. Waiting customers: %d\n", waiting);
        sem_post(&barber_sem);      // signal customer to get haircut
        sem_post(&mutex);

        // Cutting hair
        sleep(2);
        printf("Barber finished cutting hair.\n");
    }
    return NULL;
}

void *customer(void *arg) {
    int id = *((int *)arg);
    sem_wait(&mutex);
    if (waiting < CHAIRS) {
        waiting++;
        printf("Customer %d is waiting. Waiting customers: %d\n", id, waiting);
        sem_post(&customers_sem);  // notify barber
        sem_post(&mutex);

        sem_wait(&barber_sem);     // wait for barber to cut hair
        printf("Customer %d is getting a haircut.\n", id);
    } else {
        printf("Customer %d leaves, no chair available.\n", id);
        sem_post(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t barber_thread;
    pthread_t customer_threads[CUSTOMERS];
    int ids[CUSTOMERS];

    sem_init(&customers_sem, 0, 0);
    sem_init(&barber_sem, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&barber_thread, NULL, barber, NULL);

    for (int i = 0; i < CUSTOMERS; i++) {
        ids[i] = i + 1;
        pthread_create(&customer_threads[i], NULL, customer, &ids[i]);
        sleep(1);  // customers arrive at different times
    }

    for (int i = 0; i < CUSTOMERS; i++)
        pthread_join(customer_threads[i], NULL);

    // In real OS problem, barber thread runs infinitely
    // pthread_cancel(barber_thread);  // optional: stop barber
    sem_destroy(&customers_sem);
    sem_destroy(&barber_sem);
    sem_destroy(&mutex);

    return 0;
}
