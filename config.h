#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <pthread.h>

/**
 * @brief Game constants
 */
#define BOARD_SIZE 5
#define MAX_GAMES 10
#define MAX_CLIENTS 20
#define GAME_PLAYING 1
#define GAME_WAITING 0
#define GAME_OVER 2
#define GAME_NULL_ID 0
#define FIRST_PL_CHAR 'X'
#define SECOND_PL_CHAR '0'
#define EMPTY_CHAR ' '

/**
 * @brief Error states constants
 */
// Move states
#define GAME_NOT_FOUND 5
#define NOT_MY_TURN 6
#define INVALID_MOVE 7
#define FIELD_TAKEN 8

#define TRUE 1
#define FALSE 0

/**
 * @brief Messages constants
 */
#define MESSAGE_SIZE 1024
#define PLAYER_NAME_SIZE 21
#define MESS_DELIMITER ";"
#define MESS_END_CHAR "\n"
#define LOGIN_MESSAGE_SIZE (7 + PLAYER_NAME_SIZE)
#define LOGIN_MESSAGE_RESP_SIZE (9 + PLAYER_NAME_SIZE)
#define START_GAME_MESSAGE_SIZE (16 + PLAYER_NAME_SIZE)
#define MOVE_MESS_RESP_SIZE 11
#define OPP_MOVE_MESSAGE_SIZE 13
/**
 * @brief Server constants
 */
#define PORT 10000


typedef struct client client;

/**
 * @brief Struct for client
 */
typedef struct client {
    int socket;
    char username[PLAYER_NAME_SIZE];
    int current_game_id;
    int is_playing;
    int is_connected;
    char client_char;
    client *opponent;
    pthread_t *client_thread;
} client;

/**
 * @brief Struct for game
 */
typedef struct {
    int id;
    char board[BOARD_SIZE][BOARD_SIZE];
    client *player1;
    client *player2;
    client *current_player;
    int game_status;
} game;

#endif