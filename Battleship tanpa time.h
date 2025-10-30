#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 10
#define MAX_SHIPS 5
#define TRUE 1
#define FALSE 0

typedef struct {
    char symbol;
    int size;
    int hits;
    char name[20];
} Ship;

typedef struct {
    int row;
    int col;
} Coord;

void init_board(char board[SIZE][SIZE]) {
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            board[r][c] = '.';
}

void print_boards(char player_board[SIZE][SIZE], char bot_visible[SIZE][SIZE]) {
    printf("\n        --- PAPAN PEMAIN ---                   --- PAPAN LAWAN ---\n");
    printf("    ");
    for (int i = 0; i < SIZE; i++) printf(" %2d", i + 1);
    printf("           ");
    for (int i = 0; i < SIZE; i++) printf(" %2d", i + 1);
    printf("\n");

    for (int r = 0; r < SIZE; r++) {
        printf(" %c ", 'A' + r);
        for (int c = 0; c < SIZE; c++)
            printf(" %2c", player_board[r][c]);
        printf("       ");
        printf(" %c ", 'A' + r);
        for (int c = 0; c < SIZE; c++)
            printf(" %2c", bot_visible[r][c]);
        printf("\n");
    }
    printf("\nLegend: A/B/C/S/D = kapal kamu, X = hit, O = miss, . = kosong\n");
}

int is_ship_sunk(char board[SIZE][SIZE], char ship_sym) {
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            if (board[r][c] == ship_sym)
                return FALSE;
    return TRUE;
}

int remaining_ships(Ship ships[]) {
    int rem = 0;
    for (int i = 0; i < MAX_SHIPS; i++)
        if (ships[i].hits < ships[i].size)
            rem++;
    return rem;
}

int apply_attack(char board[SIZE][SIZE], Ship ships[], int r, int c, char visible_board[SIZE][SIZE]) {
    char cell = board[r][c];

    if (cell == 'X' || cell == 'O') return -1;
    if (cell == '.') {
        board[r][c] = 'O';
        if (visible_board) visible_board[r][c] = 'O';
        return 0;
    }

    board[r][c] = 'X';
    if (visible_board) visible_board[r][c] = 'X';

    int idx = -1;
    if (cell == 'A') idx = 0;
    else if (cell == 'B') idx = 1;
    else if (cell == 'C') idx = 2;
    else if (cell == 'S') idx = 3;
    else if (cell == 'D') idx = 4;

    if (idx != -1) {
        ships[idx].hits++;
        if (ships[idx].hits >= ships[idx].size && is_ship_sunk(board, cell))
            return 2; // kapal tenggelam
    }
    return 1; // kena
}

void place_random_ships(char board[SIZE][SIZE], Ship ships[]) {
    for (int i = 0; i < MAX_SHIPS; i++) {
        int placed = FALSE;
        while (!placed) {
            int r = rand() % SIZE;
            int c = rand() % SIZE;
            int orient = rand() % 2;
            int valid = TRUE;

            if (orient == 0) { // horizontal
                if (c + ships[i].size > SIZE) valid = FALSE;
                else {
                    for (int k = 0; k < ships[i].size; k++)
                        if (board[r][c + k] != '.') valid = FALSE;
                    if (valid) {
                        for (int k = 0; k < ships[i].size; k++)
                            board[r][c + k] = ships[i].symbol;
                        placed = TRUE;
                    }
                }
            } else { // vertical
                if (r + ships[i].size > SIZE) valid = FALSE;
                else {
                    for (int k = 0; k < ships[i].size; k++)
                        if (board[r + k][c] != '.') valid = FALSE;
                    if (valid) {
                        for (int k = 0; k < ships[i].size; k++)
                            board[r + k][c] = ships[i].symbol;
                        placed = TRUE;
                    }
                }
            }
        }
    }
}

/* ---------------------- BOT CERDAS (MODE TARGETING) ---------------------- */

Coord target_stack[100];
int target_top = 0;
int bot_last_hit_row = -1, bot_last_hit_col = -1;
int bot_mode_targeting = FALSE;

void push_target(int row, int col) {
    if (row >= 0 && row < SIZE && col >= 0 && col < SIZE) {
        target_stack[target_top].row = row;
        target_stack[target_top].col = col;
        target_top++;
    }
}

Coord pop_target() {
    Coord c = {-1, -1};
    if (target_top > 0) {
        target_top--;
        c = target_stack[target_top];
    }
    return c;
}

Coord bot_choose_target(char board[SIZE][SIZE]) {
    Coord c = {-1, -1};

    // Jika ada target dalam stack, ambil dari situ
    if (target_top > 0) {
        c = pop_target();
        if (board[c.row][c.col] == 'X' || board[c.row][c.col] == 'O')
            return bot_choose_target(board); // sudah diserang
        return c;
    }

    // Jika tidak ada target, pilih random
    do {
        c.row = rand() % SIZE;
        c.col = rand() % SIZE;
    } while (board[c.row][c.col] == 'X' || board[c.row][c.col] == 'O');

    return c;
}

void bot_add_adjacent_targets(int row, int col) {
    push_target(row - 1, col); // atas
    push_target(row + 1, col); // bawah
    push_target(row, col - 1); // kiri
    push_target(row, col + 1); // kanan
}

/* ---------------------- GAMEPLAY UTAMA ---------------------- */

void play_battleship() {
    char player_board[SIZE][SIZE];
    char bot_board[SIZE][SIZE];
    char bot_visible[SIZE][SIZE];

    Ship ships[MAX_SHIPS] = {
        {'A', 5, 0, "Aircraft Carrier"},
        {'B', 4, 0, "Battleship"},
        {'C', 3, 0, "Cruiser"},
        {'S', 3, 0, "Submarine"},
        {'D', 2, 0, "Destroyer"}
    };

    Ship bot_ships[MAX_SHIPS];
    for (int i = 0; i < MAX_SHIPS; i++) bot_ships[i] = ships[i];

    init_board(player_board);
    init_board(bot_board);
    init_board(bot_visible);

    srand(1234); // tanpa <time.h>

    place_random_ships(player_board, ships);
    place_random_ships(bot_board, bot_ships);

    printf("=== GAME BATTLESHIP ===\n");
    print_boards(player_board, bot_visible);

    while (remaining_ships(ships) > 0 && remaining_ships(bot_ships) > 0) {
        char input[10];
        printf("\nMasukkan koordinat serangan (contoh A5): ");
        scanf("%s", input);

        int row = toupper(input[0]) - 'A';
        int col = atoi(&input[1]) - 1;

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
            printf("Koordinat tidak valid!\n");
            continue;
        }

        int res = apply_attack(bot_board, bot_ships, row, col, bot_visible);
        if (res == -1) printf("Kamu sudah menyerang posisi itu!\n");
        else if (res == 0) printf("Meleset!\n");
        else if (res == 1) printf("Kena kapal lawan!\n");
        else if (res == 2) printf("Kapal lawan tenggelam!\n");

        /* Giliran BOT */
        Coord target = bot_choose_target(player_board);
        int r = target.row;
        int c = target.col;
        int bres = apply_attack(player_board, ships, r, c, NULL);

        printf("Bot menyerang ke %c%d... ", 'A' + r, c + 1);
        if (bres == 0) printf("melenceng!\n");
        else if (bres == 1) {
            printf("kena kapal kamu!\n");
            bot_add_adjacent_targets(r, c);
        } else if (bres == 2) {
            printf("menenggelamkan kapalmu!\n");
            target_top = 0;
        }

        print_boards(player_board, bot_visible);
    }

    if (remaining_ships(bot_ships) == 0)
        printf("\nSelamat! Kamu menang!\n");
    else
        printf("\nBot menang!\n");
}

/* ---------------------- MENU UTAMA ---------------------- */

void tampilkan_menu() {
    printf("========================================\n");
    printf("         BATTLESHIP SINGLEPLAYER        \n");
    printf("========================================\n");
    printf("1. Mulai Permainan\n");
    printf("2. Cara Bermain\n");
    printf("3. Keluar\n");
    printf("----------------------------------------\n");
    printf("Masukkan pilihan Anda: ");
}

int main() {
    int pilihan;

    while (1) {
        tampilkan_menu();
        scanf("%d", &pilihan);

        if (pilihan == 1) {
            play_battleship();
        }
        else if (pilihan == 2) {
            printf("\n----------------------------------------\n");
            printf("             CARA BERMAIN\n");
            printf("----------------------------------------\n");
            printf("- Papan 10x10 (A–J, 1–10)\n");
            printf("- Masukkan koordinat, contoh: A5 atau D10\n");
            printf("- X = Tembakan kena\n");
            printf("- O = Tembakan meleset\n");
            printf("- Tenggelamkan semua kapal lawan untuk menang!\n");
            printf("----------------------------------------\n\n");
        }
        else if (pilihan == 3) {
            printf("\n========================================\n");
            printf("Terima kasih telah bermain Battleship!\n");
            printf("Sampai jumpa di pertempuran berikutnya!\n");
            printf("========================================\n");
            exit(0);
        }
        else {
            printf("\nPilihan tidak valid! Silakan coba lagi.\n\n");
        }
    }
    return 0;
}
