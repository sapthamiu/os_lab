#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define MAX_P 10
#define MAX_R 5

int n, m;
int available[MAX_R], max_demand[MAX_P][MAX_R], allocation[MAX_P][MAX_R], need[MAX_P][MAX_R];
pthread_mutex_t lock;

bool is_safe() {
    int work[MAX_R], finish[MAX_P] = {0};
    for (int j = 0; j < m; j++) work[j] = available[j];

    int count = 0;
    while (count < n) {
        bool progress = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool can_finish = true;
                for (int j = 0; j < m; j++)
                    if (need[i][j] > work[j]) { can_finish = false; break; }
                if (can_finish) {
                    for (int j = 0; j < m; j++) work[j] += allocation[i][j];
                    finish[i] = 1;
                    progress = true;
                    count++;
                }
            }
        }
        if (!progress) return false;
    }
    return true;
}

bool request_resources(int pid, int req[]) {
    pthread_mutex_lock(&lock);

    // check if request valid
    for (int j = 0; j < m; j++) {
        if (req[j] > need[pid][j] || req[j] > available[j]) {
            pthread_mutex_unlock(&lock);
            return false;
        }
    }

    // Tentatively allocate
    for (int j = 0; j < m; j++) {
        available[j] -= req[j];
        allocation[pid][j] += req[j];
        need[pid][j] -= req[j];
    }

    bool safe = is_safe();
    if (!safe) { // rollback
        for (int j = 0; j < m; j++) {
            available[j] += req[j];
            allocation[pid][j] -= req[j];
            need[pid][j] += req[j];
        }
        printf("P%d denied (unsafe request): ", pid);
        for (int j = 0; j < m; j++) printf("%d ", req[j]);
        printf("\n");
    }
    pthread_mutex_unlock(&lock);
    return safe;
}

void release_all(int pid) {
    pthread_mutex_lock(&lock);
    for (int j = 0; j < m; j++) {
        available[j] += allocation[pid][j];
        allocation[pid][j] = 0;
    }
    pthread_mutex_unlock(&lock);
}

bool process_finished(int pid) {
    for (int j = 0; j < m; j++)
        if (need[pid][j] > 0) return false;
    return true;
}

void *process(void *arg) {
    int pid = *(int*)arg; free(arg);

    while (1) {
        if (process_finished(pid)) break;

        int req[MAX_R];
        pthread_mutex_lock(&lock);
        for (int j = 0; j < m; j++)
            req[j] = rand() % (need[pid][j] + 1);
        pthread_mutex_unlock(&lock);

        if (request_resources(pid, req)) {
            printf("P%d granted: ", pid);
            for (int j = 0; j < m; j++) printf("%d ", req[j]);
            printf("\n");

            usleep(100000 + rand() % 200000);

            if (process_finished(pid)) {
                release_all(pid);
                printf("P%d finished and released all resources.\n", pid);
                break;
            }
        }
        usleep(100000); // avoid tight loop
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    printf("Enter #processes and #resources: ");
    scanf("%d%d", &n, &m);

    printf("Enter total resources: ");
    for (int j = 0; j < m; j++) scanf("%d", &available[j]);

    printf("Enter max demand matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &max_demand[i][j]);
            need[i][j] = max_demand[i][j];
            allocation[i][j] = 0;
        }
    }

    pthread_t threads[MAX_P];
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < n; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, process, id);
    }

    for (int i = 0; i < n; i++) pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&lock);
    printf("\nAll processes finished safely.\n");
    return 0;
}
