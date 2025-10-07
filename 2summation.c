#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *arr;  
    int size;  
} ThreadData;

void* calculate_sum(void *param) {
    ThreadData *data = (ThreadData *)param;
    int *sum = (int *)malloc(sizeof(int));
    if (sum == NULL) {
        printf("Memory allocation failed\n");
        pthread_exit(NULL);
    }
    *sum = 0;
    for (int i = 0; i < data->size; i++) {
        *sum += data->arr[i];
    }
    pthread_exit((void *)sum); 
}

int main() {
    int n;

    printf("Enter the number of integers: ");
    scanf("%d", &n);

    int *numbers = (int *)malloc(n * sizeof(int));
    if (numbers == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &numbers[i]);
    }

    pthread_t thread; ThreadData data = {numbers, n}; pthread_create
    (&thread, NULL, calculate_sum, (void *)&data); void *result; pthread_join
    (thread, &result); int sum = *((int *)result); printf("Summation of the
    entered integers = %d\n", sum); free(result); free(numbers); return 0; }
