#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Structure to pass multiple parameters to the thread
typedef struct {
    int *arr;
    int size;
} ThreadData;

// Thread routine
void* sum_non_negative(void* param) {
    ThreadData* data = (ThreadData*)param;
    int *arr = data->arr;
    int size = data->size;

    int sum = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] >= 0)
            sum += arr[i];
    }

    // Dynamically allocate memory to return the result
    int* result = malloc(sizeof(int));
    *result = sum;

    pthread_exit((void*)result);  // Return the pointer to main thread
}

int main() {
    int numbers[] = {3, -1, 5, 7, -9, 2, -3, 4};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    pthread_t thread;
    ThreadData data = {numbers, size};

    // Create thread to compute sum
    if (pthread_create(&thread, NULL, sum_non_negative, &data) != 0) {
        printf("Error creating thread!\n");
        return 1;
    }

    // Wait for the thread to finish
    int* result;
    pthread_join(thread, (void**)&result);

    printf("Sum of non-negative integers: %d\n", *result);

    // Free the memory allocated inside thread
    free(result);

    return 0;
}
