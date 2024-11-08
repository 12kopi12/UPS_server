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
#define START_CHAR 'X'
#define END_CHAR '0'
#define EMPTY_CHAR ' '

#define TRUE 1
#define FALSE 0

/**
 * @brief Messages constants
 */
#define MESSAGE_SIZE 1024
#define PLAYER_NAME_SIZE 20
#define MESS_DELIMITER ";"
#define MESS_END_CHAR "\n"
#define LOGIN_MESSAGE_SIZE (7 + PLAYER_NAME_SIZE)
#define LOGIN_MESSAGE_RESP_SIZE (9 + PLAYER_NAME_SIZE)

/**
 * @brief Server constants
 */
#define PORT 10000

/**
 * @brief Struct for client
 */
typedef struct {
    int socket;
    char username[PLAYER_NAME_SIZE];
    int current_game_id;
    int is_playing;
    int is_connected;
    char client_char;
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