#ifndef __CONFIG_H__
#define __CONFIG_H__

#define PLAYER_NAME_SIZE 50
#define BOARD_SIZE 5
#define MAX_GAMES 10
#define MAX_CLIENTS 20
#define GAME_PLAYING 1
#define GAME_WAITING 0
#define GAME_OVER -1

#define PORT 10000

typedef struct {
    int socket;
    char username[PLAYER_NAME_SIZE];
    int currentGameID;
    int isPlaying;
    int isConnected;
} client;

typedef struct {
    int id;
    char board[BOARD_SIZE][BOARD_SIZE];
    client *player1;
    client *player2;
    int current_player;
    int game_status;
} game;

#endif