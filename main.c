#include <stdio.h>
#include <socket.h>
#include <pthread.h>
#include <sys/socket.h>
#include "config.h"

/**
 * Initialize the server
 * @return 0 if successful, -1 otherwise
 */
int init_server() {


    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // Set socket options for reusing the address
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    // Set the server address and port
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Socket binding failed");
        return -1;
    }
    printf("Server is running on port %d\n", PORT);

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Socket listening failed");
        close(server_socket);
        return -1;
    }

    // Accept incoming connections
    struct sockaddr_in client_address;
    int client_address_len = sizeof(client_address);
    while (1) {
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&client_address_len);
        if (client_socket == -1) {
            perror("Socket accepting failed");
        } else {
            printf("Client connected\n");
            // Handle the client in a separate thread
            pthread_t client_thread;
            // todo handle_client
            if (pthread_create(&client_thread, NULL, handle_client, &client_socket) != 0) {
                perror("Thread creation failed");
                close(client_socket);
                close(server_socket);
                return -1;
            }

        }
    }


}


int main(void) {
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, init_server, PORT);
    return 0;
}
