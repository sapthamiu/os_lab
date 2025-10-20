#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 2025

struct game {
    char board[3][3];
    int turn;       // 1 = Player1, 2 = Player2
    int game_over;  // 0 = running, 1 = ended
};

// Function to print board
void print_board(char board[3][3]) {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %c ", board[i][j]);
            if (j < 2) printf("|");
        }
        printf("\n");
        if (i < 2) printf("---+---+---\n");
    }
    printf("\n");
}

// Function to check winner
int check_winner(char board[3][3]) {
    // Rows & Columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != ' ' &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2]) return 1;
        if (board[0][i] != ' ' &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i]) return 1;
    }
    // Diagonals
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) return 1;
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) return 1;

    // Check for draw
    int filled = 1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ') filled = 0;
    if (filled) return 2; // draw

    return 0; // no winner yet
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <player_number (1 or 2)>\n", argv[0]);
        exit(1);
    }

    int player = atoi(argv[1]);
    char symbol = (player == 1) ? 'X' : 'O';

    int shmid = shmget(SHM_KEY, sizeof(struct game), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    struct game *g = (struct game *)shmat(shmid, NULL, 0);
    if (g == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Player 1 initializes the game
    if (player == 1) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                g->board[i][j] = ' ';
        g->turn = 1;
        g->game_over = 0;
        printf("Game initialized. You are Player 1 (X).\n");
    } else {
        printf("You are Player 2 (O).\n");
    }

    while (!g->game_over) {
        if (g->turn == player) {
            print_board(g->board);
            int r, c;
            do {
                printf("Player %d (%c), enter row and column (0-2 0-2): ",
                       player, symbol);
                scanf("%d %d", &r, &c);
            } while (r < 0 || r > 2 || c < 0 || c > 2 || g->board[r][c] != ' ');

            g->board[r][c] = symbol;
            int result = check_winner(g->board);
            if (result == 1) {
                print_board(g->board);
                printf("Player %d (%c) WINS!\n", player, symbol);
                g->game_over = 1;
            } else if (result == 2) {
                print_board(g->board);
                printf("It's a DRAW!\n");
                g->game_over = 1;
            } else {
                g->turn = (player == 1) ? 2 : 1;
            }
        } else {
            sleep(1); // wait for opponent
        }
    }

    if (player == 1) {
        shmctl(shmid, IPC_RMID, NULL); // cleanup only once
    }
    shmdt(g);

    return 0;
}
