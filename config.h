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
#define GAME_OVER -1

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
#define INIT_MESSAGE "TIC-TAC-TOE;INIT\n"

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
    int currentGameID;
    int isPlaying;
    int isConnected;
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
    int current_player;
    int game_status;
} game;

#endif