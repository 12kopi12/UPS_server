#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include "communication.h"
#include "client_manager.h"

void receive_messages(client *cl) {
//    client *cl = (client *) arg;
    int client_socket = cl->socket;
    char message[MESSAGE_SIZE] = {0};
    int valread;
    while (1) {
        valread = recv(client_socket, message, sizeof(message), 0);
//        if (valread <= 0) {
//            printf("Client %s disconnected\n", cl->username);
//            close(client_socket);
//            remove_client(cl);
//            break;
//        }
        serve_message(cl, message);
        printf("Client mess: %s\n", message);
        memset(message, 0, sizeof(message));
        valread = 0;
    }
    printf("Client %s disconnected\n", cl->username);
}

void serve_message(client *cl, char *message) {

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

