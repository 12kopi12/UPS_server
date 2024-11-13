#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include "communication.h"
#include "client_manager.h"
#include "game_logic.h"

void receive_messages(client *cl) {
//    client *cl = (client *) arg;
    int client_socket = cl->socket;
    char message[MESSAGE_SIZE] = {0};
    int valread;
    while (1) {
        valread = recv(client_socket, message, sizeof(message), 0);
        if (valread <= 0) {
            printf("Client %s disconnected\n", cl->username);
            close(client_socket);
            remove_client(cl);
            break;
        }
        printf("Client mess: %s -> %s\n", message, cl->username);
        serve_message(cl, message);
        memset(message, 0, sizeof(message));
        valread = 0;
    }
    printf("Client %s disconnected\n", cl->username);
}

void serve_message(client *cl, char *message) {
    // get the first token
    char *token = strtok(message, MESS_DELIMITER);

    // check message type
    if (strcmp(token, "MOVE") == 0) {
        // get the second token
//        token = strtok(NULL, MESS_DELIMITER);
        int x = atoi(strtok(NULL, MESS_DELIMITER));
//        token = strtok(NULL, MESS_DELIMITER);
        int y = atoi(strtok(NULL, MESS_DELIMITER));
        int move_status = validate_move(cl, x, y);
        int game_status = validate_game_status(cl, x, y);

        move_response(cl, move_status, x, y);
//        usleep(10000);
        game_status_response(cl, game_status);
    } else if (strcmp(token, "WANT_GAME") == 0) {
        want_game_response(cl);
    } else if (strcmp(token, "LOGOUT") == 0) {
        pthread_t thread = *cl->client_thread;
        remove_client(cl);
        pthread_join(thread, NULL);
    }
}

void *send_mess_by_socket(int socket, char *mess) {
    printf("Sending message: %s\n", mess);
    send(socket, mess, strlen(mess), 0);
    return NULL;
}

void *send_mess(client *client, char *mess) {
    printf("Sending message: %s -> to client %s\n", mess, client->username);
    int client_socket = client->socket;
    send(client_socket, mess, strlen(mess), 0);
    return NULL;
}


void move_response(client *cl, int status, int x, int y) {
    char response[MOVE_MESS_RESP_SIZE] = {0};

    if (status == TRUE) {
        sprintf(response, "MOVE;%c;%c;%c\n", status + '0', x + '0', y + '0');
        send_mess(cl, response);

        char mess[OPP_MOVE_MESSAGE_SIZE] = {0};
        sprintf(mess, "OPP_MOVE;%c;%c\n", x + '0', y + '0');
        send_mess(cl->opponent, mess);
    }
    else {
        sprintf(response, "MOVE;%c;0;0\n", status + '0');
        send_mess(cl, response);
    }
}

void game_status_response(client *cl, int status) {
    char response[GAME_STATUS_RESP_SIZE] = {0};

    if (status == GAME_WIN) {
        sprintf(response, "GAME_STATUS;%s\n", cl->username);
        send_mess(cl->opponent, response);
        send_mess(cl, response);
        remove_game(cl);
        clean_client_game(cl->opponent);
        clean_client_game(cl);
    } else if (status == GAME_DRAW) {
        sprintf(response, "GAME_STATUS;DRAW\n");
        send_mess(cl->opponent, response);
        send_mess(cl, response);
        remove_game(cl);
        clean_client_game(cl->opponent);
        clean_client_game(cl);
    }
}

void want_game_response(client *cl) {
    printf("Client %s wants to play\n", cl->username);
    set_want_game(cl, TRUE);
    int found = find_waiting_player(cl);

    char response[WANT_GAME_RESP_SIZE] = {0};
    if (found == FALSE) {
        sprintf(response, "WANT_GAME;%c\n", FIRST_PL_CHAR);
    } else {
        sprintf(response, "WANT_GAME;%c\n", SECOND_PL_CHAR);
    }
    send_mess(cl, response);

    if (found == TRUE) {
        // Start the game
        char game_response[START_GAME_MESSAGE_SIZE] = {0};
        sprintf(game_response, "START_GAME;%s;%c;%c\n", cl->opponent->username, cl->opponent->client_char, cl->is_playing ? '1' : '0');
        send_mess(cl, game_response);
    }
}