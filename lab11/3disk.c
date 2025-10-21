#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct DS {
    int id, at, cyl, addr, pid;
};

int cmpArr(const void *a, const void *b) {
    return ((struct DS*)a)->at - ((struct DS*)b)->at;
}

void FCFS(struct DS req[], int n, int head) {
    int move = 0;
    qsort(req, n, sizeof(struct DS), cmpArr);
    printf("\n--- FCFS ---\nSeq: ");
    for (int i = 0; i < n; i++) {
        move += abs(head - req[i].cyl);
        head = req[i].cyl;
        printf("%d ", head);
    }
    printf("\nTotal movement = %d\n", move);
}

void SSTF(struct DS req[], int n, int head) {
    int move = 0, done = 0, used[100] = {0};
    printf("\n--- SSTF ---\nSeq: ");
    while (done < n) {
        int min = INT_MAX, next = -1;
        for (int i = 0; i < n; i++)
            if (!used[i] && abs(head - req[i].cyl) < min)
                min = abs(head - req[i].cyl), next = i;
        move += min; head = req[next].cyl; used[next] = 1; done++;
        printf("%d ", head);
    }
    printf("\nTotal movement = %d\n", move);
}

int main() {
    int n, head;
    printf("Enter number of requests: ");
    scanf("%d", &n);
    struct DS r[n];
    for (int i = 0; i < n; i++) {
        printf("Req %d → Arrival, Cylinder, Addr, PID: ", i + 1);
        scanf("%d%d%d%d", &r[i].at, &r[i].cyl, &r[i].addr, &r[i].pid);
        r[i].id = i + 1;
    }
    printf("Enter initial head position: ");
    scanf("%d", &head);
    FCFS(r, n, head);
    SSTF(r, n, head);
}
