#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

#include "config.h"

/**
 * @brief Validates the move of the player and updates the game board
 * @param cl client who made the move
 * @param x x coordinate
 * @param y y coordinate
 * @return TRUE if the move was successful, Error states for move otherwise
 */
int validate_move(client *cl, int x, int y);

#endif