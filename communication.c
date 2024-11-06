#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include "communication.h"
#include "client_manager.h"

void *receive_messages(void *arg) {
    client *cl = (client *) arg;
    int client_socket = cl->socket;
    char message[MESSAGE_SIZE] = {0};
    int valread;
    while (1) {
        valread = recv(client_socket, message, sizeof(message), 0);
        if (valread <= 0) {
            printf("Client disconnected\n");
            close(client_socket);
            remove_client(cl);
            break;
        }
        printf("Client: %s\n", message);
        memset(message, 0, sizeof(message));
    }
    return NULL;
}