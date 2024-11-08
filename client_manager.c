#include "client_manager.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include "config.h"
#include "communication.h"

// Mutex for clients array
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
// Array of all clients
client *clients[MAX_CLIENTS] = {0};

int add_client(int socket, char *username, pthread_t *thread) {
    if (get_client_by_socket(socket) != NULL) {
        return FALSE;
    }

    pthread_mutex_lock(&clients_mutex);

    // check capacity
    int count = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL /*&& clients[i]->is_connected*/) {
            count++;
        }
    }
    if (count >= MAX_CLIENTS) {
        pthread_mutex_unlock(&clients_mutex);
        return FALSE;
    }


    client *cl = malloc(sizeof(client));
    if (cl == NULL) {
        perror("Failed to allocate memory for client");
        pthread_mutex_unlock(&clients_mutex);
        return FALSE;
    }


    cl->socket = socket;
    cl->is_playing = FALSE;
    cl->is_connected = TRUE;
    cl->client_thread = thread;
    cl->current_game_id = GAME_NULL_ID;
    strcpy(cl->username, username);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == NULL) {
            clients[i] = cl;
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
    return TRUE;
}

int find_waiting_player(client *cl) {
    pthread_mutex_lock(&clients_mutex);
    int found = FALSE;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL && strcmp(clients[i]->username, cl->username) != 0 && clients[i]->is_connected && clients[i]->current_game_id == GAME_NULL_ID) {
            // clients[i] already waits -> has 'X'
            clients[i]->client_char = 'X';
            clients[i]->is_playing = TRUE;

            // Start the game
            cl->client_char = 'O';
            cl->is_playing = FALSE;

            game *new_game = create_new_game(clients[i], cl);

            cl->current_game_id = new_game->id;
            clients[i]->current_game_id = new_game->id;

            send_mess(clients[i], "START_GAME\n");

            found = TRUE;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    return found;
}


client *get_client_by_socket(int socket) {
    pthread_mutex_lock(&clients_mutex);
    client *result = NULL;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL && clients[i]->socket == socket) {
            result = clients[i];
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    return result;
}

int get_connected_clients_count() {
    int count = 0;
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL /*&& clients[i]->is_connected*/) {
            count++;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    return count;
}

void print_clients() {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL /*&& clients[i]->is_connected*/) {
            printf("Client: %s; Game: %d; Socket: %d\n", clients[i]->username, clients[i]->current_game_id, clients[i]->socket);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

int remove_client(client *cl) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == cl) {
            if (cl->client_thread != NULL) {
                pthread_cancel(*cl->client_thread);
            }
            free(clients[i]);
            clients[i] = NULL;
            pthread_mutex_unlock(&clients_mutex);
            return TRUE;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
    return FALSE;
}

int remove_client_by_socket(int socket) {
    client *cl = get_client_by_socket(socket);
    if (cl == NULL) {
        return FALSE;
    }
    return remove_client(cl);
}

void *run_client(void *arg) {
    client *cl = (client *) arg;

    int found = find_waiting_player(cl);

    char response[LOGIN_MESSAGE_RESP_SIZE] = {0};
    if (found == FALSE) {
        sprintf(response, "LOGIN;%s;%c\n", cl->username, START_CHAR);
    } else {
        sprintf(response, "LOGIN;%s;%c\n", cl->username, END_CHAR);
    }
    send_mess(cl, response);

    if (found == TRUE) {
        // Start the game
        send_mess(cl, "START_GAME\n");
    }

    receive_messages(cl);
}
