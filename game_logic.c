#include "game_logic.h"
#include "game_manager.h"
#include "client_manager.h"
#include "communication.h"
#include <stdio.h>

int validate_move(client *cl, int x, int y) {
    game *g = get_game_by_id(cl->current_game_id);
    if (g == NULL) {
        perror("Game not found");
        return GAME_NOT_FOUND;
    }

    if (g->game_status != GAME_PLAYING) {
        perror("Game is not in progress");
        return GAME_NOT_FOUND;
    }

    if (g->current_player != cl) {
        perror("It's not your turn");
        return NOT_MY_TURN;
    }

    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        perror("Invalid move");
        return INVALID_MOVE;
    }

    if (g->board[x][y] != EMPTY_CHAR) {
        perror("Field is already taken");
        return FIELD_TAKEN;
    }

    g->board[x][y] = cl->client_char;

    return TRUE;
}