#include "game_logic.h"
#include "game_manager.h"
#include "client_manager.h"
#include "communication.h"
#include <stdio.h>

int count_in_direction(char player, int x, int y, int dx, int dy, game *g) {
    int count = 0;

    // positive direction
    for (int i = 0; i < WINNING_LENGTH; i++) {
        int nx = x + i * dx;
        int ny = y + i * dy;
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && g->board[ny][nx] == player) {
            count++;
        } else {
            break;
        }
    }

    // negative direction
    for (int i = 1; i < WINNING_LENGTH; i++) {
        int nx = x - i * dx;
        int ny = y - i * dy;
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && g->board[ny][nx] == player) {
            count++;
        } else {
            break;
        }
    }

    return count;
}

int validate_game_status(client *cl, int x, int y) {
    game *g = get_game_by_id(cl->current_game_id);
    pthread_mutex_lock(&mutex_games);
    // look from the last move to all directions
    int directions[4][2] = {
            {1, 0},  // horizontal
            {0, 1},  // vertical
            {1, 1},  // diagonal (/)
            {1, -1}  // diagonal (\)
    };

    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        int count = count_in_direction(cl->client_char, x, y, dx, dy, g);

        // if number of player's chars in the direction is equal or
        //   higher to WINNING_LENGTH, player wins
        if (count >= WINNING_LENGTH) {
            g->game_status = GAME_OVER;
            g->winner = cl;
            pthread_mutex_unlock(&mutex_games);
            return GAME_WIN;
        }
    }

    // check if the board is full
    int empty_fields = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (g->board[i][j] == EMPTY_CHAR) {
                empty_fields++;
            }
        }
    }
    if (empty_fields == 0) {
        g->game_status = GAME_OVER;
        g->winner = NULL;
        pthread_mutex_unlock(&mutex_games);
        return GAME_DRAW;
    }

    pthread_mutex_unlock(&mutex_games);
    return 0;
}

int validate_move(client *cl, int x, int y) {
    game *g = get_game_by_id(cl->current_game_id);
    pthread_mutex_lock(&mutex_games);
    if (g == NULL) {
        perror("Game not found");
        pthread_mutex_unlock(&mutex_games);
        return GAME_NOT_FOUND;
    }

    if (g->game_status != GAME_PLAYING) {
        perror("Game is not in progress");
        pthread_mutex_unlock(&mutex_games);
        return GAME_NOT_FOUND;
    }

    if (g->current_player != cl) {
        perror("It's not your turn");
        pthread_mutex_unlock(&mutex_games);
        return NOT_MY_TURN;
    }

    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        perror("Invalid move");
        pthread_mutex_unlock(&mutex_games);
        return INVALID_MOVE;
    }

    if (g->board[y][x] != EMPTY_CHAR) {
        perror("Field is already taken");
        pthread_mutex_unlock(&mutex_games);
        return FIELD_TAKEN;
    }

    g->current_player = cl == g->player1 ? g->player2 : g->player1;
    g->board[y][x] = cl->client_char;

    pthread_mutex_unlock(&mutex_games);
    return TRUE;
}
