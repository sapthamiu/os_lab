#include<stdio.h>
#include<stdlib.h>

int main(){
    int queue[100], seek = 0;
    int n, head;
    printf("Enter size of queue: ");
    scanf("%d", &n);
    printf("Enter the queue:\n");
    for(int i = 1; i <= n; i++)
        scanf("%d", &queue[i]);
    printf("Enter the initial head position: ");
    scanf("%d", &head);
    queue[0] = head;
    printf("\n");
    for(int j = 0; j <= n-1; j++){
        int diff = abs(queue[j+1]-queue[j]);
        seek += diff;
        printf("Move from %d to %d with seek %d\n", queue[j], queue[j+1], diff);
    }
    printf("Total seek time is %d\n", seek);
    float avg = seek/(float)n;
    printf("Average seek time is %f", avg);
    return 0;
}