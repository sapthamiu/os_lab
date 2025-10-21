#include <stdio.h>

#define P 5
#define R 3

void calcNeed(int n[P][R], int m[P][R], int a[P][R]) {
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            n[i][j] = m[i][j] - a[i][j];
}

int isSafe(int avail[R], int m[P][R], int a[P][R], int seq[P]) {
    int n[P][R], f[P] = {0}, w[R]; 
    calcNeed(n, m, a);
    for (int i = 0; i < R; i++) w[i] = avail[i];
    int cnt = 0;
    while (cnt < P) {
        int found = 0;
        for (int p = 0; p < P; p++) {
            if (!f[p]) {
                int j;
                for (j = 0; j < R && n[p][j] <= w[j]; j++);
                if (j == R) {
                    for (int k = 0; k < R; k++) w[k] += a[p][k];
                    seq[cnt++] = p; f[p] = found = 1;
                }
            }
        }
        if (!found) return 0;
    }
    return 1;
}

void reqRes(int p, int req[R], int avail[R], int m[P][R], int a[P][R]) {
    printf("\nP%d requesting: ", p);
    for (int i = 0; i < R; i++) printf("%d ", req[i]);
    printf("\n");

    int n[P][R]; calcNeed(n, m, a);
    for (int i = 0; i < R; i++)
        if (req[i] > n[p][i]) return printf("Error: exceeds max claim\n"), (void)0;
    for (int i = 0; i < R; i++)
        if (req[i] > avail[i]) return printf("Error: insufficient resources\n"), (void)0;

    int ta[R], taP[P][R];
    for (int i = 0; i < R; i++) ta[i] = avail[i] - req[i];
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            taP[i][j] = a[i][j] + (i == p ? req[j] : 0);

    int seq[P];
    if (isSafe(ta, m, taP, seq)) {
        printf("Request granted. Safe sequence: < ");
        for (int i = 0; i < P; i++) printf("P%d ", seq[i]);
        printf(">\n");
        for (int i = 0; i < R; i++) {
            avail[i] = ta[i];
            a[p][i] = taP[p][i];
        }
    } else printf("Unsafe state! Request denied.\n");
}

void printMatrix(const char *name, int x[P][R]) {
    printf("\n%s:\n   A  B  C\n", name);
    for (int i = 0; i < P; i++) {
        printf("P%d ", i);
        for (int j = 0; j < R; j++) printf("%2d ", x[i][j]);
        printf("\n");
    }
}

int main() {
    int alloc[P][R]={{0,1,0},{2,0,0},{3,0,2},{2,1,1},{0,0,2}},
        max[P][R]={{7,5,3},{3,2,2},{9,0,2},{2,2,2},{4,3,3}},
        avail[R]={3,3,2}, need[P][R], seq[P];

    calcNeed(need,max,alloc);
    printMatrix("Initial Need", need);
    printf("\nChecking initial safety...\n");
    if (isSafe(avail,max,alloc,seq)) {
        printf("Safe state. Seq: < ");
        for (int i=0;i<P;i++) printf("P%d ", seq[i]);
        printf(">\n");
    } else printf("Unsafe state!\n");

    char ch='y'; int p, req[R];
    while (ch=='y'||ch=='Y') {
        printf("\nMake a request? (y/n): ");
        scanf(" %c", &ch);
        if (ch!='y'&&ch!='Y') break;
        printf("Process num (0-%d): ", P-1); scanf("%d",&p);
        printf("Request for A B C: "); scanf("%d%d%d",&req[0],&req[1],&req[2]);
        reqRes(p,req,avail,max,alloc);
        printMatrix("Allocation",alloc);
        calcNeed(need,max,alloc);
        printMatrix("Need",need);
        printf("Available: "); for(int i=0;i<R;i++) printf("%2d ",avail[i]); printf("\n");
    }
    printf("\nExiting...\n");
}
