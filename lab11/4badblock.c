#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
int *disk, D, SPARE, bad_count = 0, file_count = 0;

typedef struct { int bad, spare; } Bad;
typedef struct { int id, start, len; } File;

Bad bads[MAX]; File files[MAX];

void init() {
    int spare, nbad;
    printf("Total blocks: "); scanf("%d", &D);
    printf("Spare blocks: "); scanf("%d", &spare);
    SPARE = D - spare; disk = calloc(D, sizeof(int));
    for (int i = SPARE; i < D; i++) disk[i] = -2;
    printf("Bad blocks count: "); scanf("%d", &nbad);
    printf("Enter bad block indices: ");
    for (int i = 0; i < nbad; i++) {
        int x; scanf("%d", &x);
        if (x < SPARE) disk[x] = -1;
    }
    printf("Disk initialized.\n");
}

int handleBad(int b) {
    for (int i = SPARE; i < D; i++)
        if (disk[i] == -2) {
            disk[i] = -3;
            bads[bad_count++] = (Bad){b, i};
            printf("Mapped Bad %d → Spare %d\n", b, i);
            return i;
        }
    printf("No spare left!\n"); return -1;
}

void allocFile() {
    if (file_count >= MAX) return;
    int id, len; printf("File ID len: "); scanf("%d%d", &id, &len);
    for (int i = 0; i <= SPARE - len; i++) {
        int ok = 1;
        for (int j = 0; j < len; j++) if (disk[i+j]) { ok = 0; i += j; break; }
        if (ok) {
            for (int j = 0; j < len; j++) disk[i+j] = id;
            files[file_count++] = (File){id, i, len};
            printf("File %d → start %d\n", id, i);
            return;
        }
    }
    printf("No space!\n");
}

void readFile() {
    int id; printf("Read file ID: "); scanf("%d", &id);
    File *f = NULL; for (int i = 0; i < file_count; i++) if (files[i].id == id) f = &files[i];
    if (!f) return printf("File not found!\n"), (void)0;
    for (int i = 0; i < f->len; i++) {
        int blk = f->start + i, spare = blk, redirected = 0;
        for (int j = 0; j < bad_count; j++)
            if (bads[j].bad == blk) { spare = bads[j].spare; redirected = 1; }
        if (disk[blk] == -1 && !redirected) spare = handleBad(blk);
        printf("Block %d → %s %d\n", blk, redirected || disk[blk]==-1 ? "Spare" : "Read", spare);
    }
}

void showBad() {
    printf("\nBad | Spare\n----------\n");
    for (int i = 0; i < bad_count; i++)
        printf("%3d | %5d\n", bads[i].bad, bads[i].spare);
}

void showDisk() {
    printf("\nDisk Layout:\n");
    for (int i = 0; i < D; i++) {
        printf("%2d:", i);
        if (disk[i] == 0) printf("Free ");
        else if (disk[i] == -1) printf("Bad ");
        else if (disk[i] == -2) printf("Spare ");
        else if (disk[i] == -3) printf("UsedSpare ");
        else printf("F%d ", disk[i]);
        if (i % 10 == 9) printf("\n");
    }
}

int main() {
    int ch;
    init();
    while (1) {
        printf("\n1.Allocate 2.Read 3.ShowBad 4.ShowDisk 5.Exit: ");
        scanf("%d", &ch);
        if (ch == 1) allocFile();
        else if (ch == 2) readFile();
        else if (ch == 3) showBad();
        else if (ch == 4) showDisk();
        else break;
    }
    free(disk);
}
