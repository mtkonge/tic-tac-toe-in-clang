#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { EMPTY, X, O } cell_t;

typedef enum { PLAYER_X, PLAYER_O } player_t;

bool has_won(player_t turn, cell_t board[9]) {
    static const cell_t player_to_cell[] = {
        [PLAYER_X] = X,
        [PLAYER_O] = O,
    };

    cell_t cell_turn = player_to_cell[turn];
    for (int i = 0; i < 3; i++) {
        // horizontals
        if (board[0 + (i * 3)] == cell_turn && board[1 + (i * 3)] == cell_turn &&
            board[2 + (i * 3)] == cell_turn) {
            return true;
        }
        // verticals
        if (board[0 + i] == cell_turn && board[3 + i] == cell_turn && board[6 + i] == cell_turn) {
            return true;
        }
    }
    // crosses
    if (board[0] == cell_turn && board[4] == cell_turn && board[8] == cell_turn ||
        board[2] == cell_turn && board[4] == cell_turn && board[6] == cell_turn) {
        return true;
    }

    return false;
}

bool is_tie(cell_t board[9]) {
    for (int i = 0; i < 9; i++) {
        if (board[i] == EMPTY) {
            return false;
        } else if (i == 9 - 1) {
            return true;
        }
    }
    fprintf(stderr, "ERROR: Unreachable\n");
    exit(EXIT_FAILURE);
}

void copy_board_to_chars(cell_t board[9], char dst[9]) {
    for (int i = 0; i < 9; i++) {
        if (board[i] == X) {
            dst[i] = 'X';
        } else if (board[i] == O) {
            dst[i] = 'O';
        } else {
            dst[i] = i + '0';
        }
    }
}

void print_board(cell_t board[9]) {
    char char_board[9];
    copy_board_to_chars(board, char_board);

    printf("\n"
           "|%c|%c|%c|\n"
           "|%c|%c|%c|\n"
           "|%c|%c|%c|\n",
           char_board[0], char_board[1], char_board[2], char_board[3], char_board[4], char_board[5],
           char_board[6], char_board[7], char_board[8]);
}

void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // do nothing
    }
}

char player_char(player_t player) {
    if (player == PLAYER_X) {
        return 'X';
    } else {
        return 'O';
    }
}

cell_t player_cell(player_t player) {
    if (player == PLAYER_X) {
        return X;
    } else {
        return O;
    }
}

void mutate_board(player_t *current_turn, cell_t board[9], int picked_cell_id) {
    if (*current_turn == PLAYER_X) {
        board[picked_cell_id] = X;
        *current_turn = PLAYER_O;
    } else {
        board[picked_cell_id] = O;
        *current_turn = PLAYER_X;
    }
}

void change_turns(player_t *current_turn) {
    if (*current_turn == PLAYER_X) {
        *current_turn = PLAYER_O;
    } else {
        *current_turn = PLAYER_X;
    }
}

bool is_valid_input(int picked_cell_id, cell_t board[9]) {
    if (picked_cell_id < 0 || picked_cell_id > 8) {
        printf("A space with the id %d is not a valid\n", picked_cell_id);
        return false;
    } else if (board[picked_cell_id] != EMPTY) {
        printf("The space %d is already occupied\n", picked_cell_id);
        return false;
    }
    return true;
}

int main() {
    cell_t board[9];
    for (int i = 0; i < sizeof(board) / sizeof(board[0]); i++) {
        board[i] = EMPTY;
    }
    player_t current_turn = PLAYER_X;
    int picked_cell_id;

    while (true) {
        print_board(board);
        printf("Pick a space on the board from 0-8 (%c's turn): ", player_char(current_turn));
        if (scanf("%d", &picked_cell_id) != 1) {
            printf("Invalid input, please enter a number\n");
            clear_input();
            continue;
        }

        is_valid_input(picked_cell_id, board);

        board[picked_cell_id] = player_cell(current_turn);

        if (has_won(current_turn, board)) {
            print_board(board);
            printf("Player %c won the game\n", player_char(current_turn));
            exit(0);
        }

        if (is_tie(board)) {
            print_board(board);
            printf("No available cells left, it's a tie \n");
            exit(0);
        }

        change_turns(&current_turn);
    }
}
