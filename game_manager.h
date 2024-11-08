/**
* Define logic for the game Tic-Tac-Toe
*/
#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

#include "config.h"


/**
 * @brief Creates a new game
 * @param player_1 player who started the game
 * @param player_2 player who joined the game
 * @return pointer to the created game
 */
game *create_new_game(client *player_1, client *player_2);

/**
 * @brief Creates and initialize the game board
 * @param board game board
 * @return initialized game board with empty fields
 */
void initialize_board(char board[BOARD_SIZE][BOARD_SIZE]);

#endif