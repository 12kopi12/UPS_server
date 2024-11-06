#include "client_manager.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>


// Array of all clients
client *clients[MAX_CLIENTS];

// Mutex for clients array
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

int add_client(int socket, char *username, pthread_t *thread) {
    if (get_client_by_socket(socket) != NULL) {
        return FALSE;
    }

    pthread_mutex_lock(&clients_mutex);

    // check capacity
    int count = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL /*&& clients[i]->isConnected*/) {
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
    cl->currentGameID = -1;
    cl->isPlaying = FALSE;
    cl->isConnected = TRUE;
    cl->client_thread = thread;
    strcpy(cl->username, username);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == NULL) {
            clients[i] = cl;
            pthread_mutex_unlock(&clients_mutex);
            return TRUE;
        }
    }

//    free(cl);
    pthread_mutex_unlock(&clients_mutex);
    return FALSE;
}

int remove_client(client *cl) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == cl) {
            pthread_cancel(*cl->client_thread);
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
        if (clients[i] != NULL /*&& clients[i]->isConnected*/) {
            count++;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    return count;
}
