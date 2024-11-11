#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

#include "config.h"

/**
 * @brief Count the number of player's chars in the direction
 * @param player player's char
 * @param x player's last x move
 * @param y player's last y move
 * @param dx direction x
 * @param dy direction y
 * @param g game
 * @return number of player's chars in the direction
 */
int count_in_direction(char player, int x, int y, int dx, int dy, game *g);

/**
 * @brief Validates the move of the player and updates the game board
 * @param cl client who made the move
 * @param x x coordinate
 * @param y y coordinate
 * @return TRUE if the move was successful, Error states for move otherwise
 */
int validate_move(client *cl, int x, int y);

/**
 * @brief Validates the game status
 * @param cl client who made the move
 * @param x x coordinate (last move)
 * @param y y coordinate (last move)
 * @return 0 game is not finished, 1 game wins someone, 2 game is draw
 */
int validate_game_status(client *cl, int x, int y);

#endif