#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to pass data to the thread
typedef struct {
    int start;
    int end;
} ThreadData;

// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0)
            return false;
    }
    return true;
}

// Thread function to find primes
void* generatePrimes(void* param) {
    ThreadData* data = (ThreadData*)param;
    int start = data->start;
    int end = data->end;

    // Allocate space to store primes
    int* primes = malloc((end - start + 1) * sizeof(int));
    int count = 0;

    for (int i = start; i <= end; i++) {
        if (isPrime(i)) {
            primes[count++] = i;
        }
    }

    // Store the count as the first element (for communication)
    int* result = malloc((count + 1) * sizeof(int));
    result[0] = count;
    for (int i = 0; i < count; i++) {
        result[i + 1] = primes[i];
    }

    free(primes);
    pthread_exit((void*)result); // Return primes to main thread
}

int main() {
    int start, end;
    printf("Enter starting number: ");
    scanf("%d", &start);
    printf("Enter ending number: ");
    scanf("%d", &end);

    pthread_t thread;
    ThreadData data = {start, end};

    // Create thread to generate primes
    if (pthread_create(&thread, NULL, generatePrimes, &data) != 0) {
        printf("Error creating thread!\n");
        return 1;
    }

    // Wait for thread to finish and get result
    int* result;
    pthread_join(thread, (void**)&result);

    int count = result[0];
    printf("\nPrime numbers between %d and %d are:\n", start, end);
    for (int i = 1; i <= count; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    free(result);
    return 0;
}
