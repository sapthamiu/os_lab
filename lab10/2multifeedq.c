#include <stdio.h>
#include <stdlib.h>

typedef struct Process {
    int pid;            // process ID
    int burstTime;      // total CPU burst time
    int remainingTime;  // time left to execute
    int priority;       // lower value = higher priority
    struct Process *next;
} Process;

// ==== Queue Implementation ====
typedef struct Queue {
    Process *front;
    Process *rear;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = NULL;
}

int isEmpty(Queue *q) {
    return q->front == NULL;
}

void enqueue(Queue *q, Process *p) {
    p->next = NULL;
    if (q->rear == NULL)
        q->front = q->rear = p;
    else {
        q->rear->next = p;
        q->rear = p;
    }
}

Process* dequeue(Queue *q) {
    if (isEmpty(q)) return NULL;
    Process *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    return temp;
}

// ==== Round Robin for Queue 1 ====
void roundRobin(Queue *q1, Queue *q2, int timeQuantum) {
    while (!isEmpty(q1)) {
        Process *p = dequeue(q1);
        printf("Q1: Running P%d for ", p->pid);
        if (p->remainingTime > timeQuantum) {
            printf("%d units (remaining %d)\n", timeQuantum, p->remainingTime - timeQuantum);
            p->remainingTime -= timeQuantum;
            // move to next queue (Q2)
            enqueue(q2, p);
        } else {
            printf("%d units (completed)\n", p->remainingTime);
            p->remainingTime = 0;
            free(p);
        }
    }
}

// ==== Priority Scheduling for Queue 2 ====
void priorityScheduling(Queue *q2, Queue *q3) {
    while (!isEmpty(q2)) {
        // find highest priority process
        Process *prev = NULL, *highestPrev = NULL, *highest = q2->front;
        Process *curr = q2->front;
        while (curr != NULL) {
            if (curr->priority < highest->priority) {
                highest = curr;
                highestPrev = prev;
            }
            prev = curr;
            curr = curr->next;
        }
        // remove highest priority process from queue
        if (highestPrev == NULL)
            q2->front = highest->next;
        else
            highestPrev->next = highest->next;
        if (highest == q2->rear) q2->rear = highestPrev;

        printf("Q2: Running P%d (priority %d)\n", highest->pid, highest->priority);

        // execute fully (no preemption)
        highest->remainingTime = 0;
        free(highest);
    }
}

// ==== FCFS for Queue 3 ====
void fcfs(Queue *q3) {
    while (!isEmpty(q3)) {
        Process *p = dequeue(q3);
        printf("Q3: Running P%d (FCFS - burst %d)\n", p->pid, p->burstTime);
        free(p);
    }
}

// ==== Main Scheduling Logic ====
int main() {
    Queue q1, q2, q3;
    initQueue(&q1);
    initQueue(&q2);
    initQueue(&q3);

    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        Process *p = (Process *)malloc(sizeof(Process));
        p->pid = i + 1;
        printf("Enter burst time and priority for P%d: ", p->pid);
        scanf("%d %d", &p->burstTime, &p->priority);
        p->remainingTime = p->burstTime;
        p->next = NULL;
        enqueue(&q1, p); // all processes start in Q1
    }

    printf("\n--- Starting Multilevel Feedback Queue Scheduling ---\n");
    roundRobin(&q1, &q2, 4);
    priorityScheduling(&q2, &q3);
    fcfs(&q3);

    printf("\nAll processes completed.\n");
    return 0;
}
