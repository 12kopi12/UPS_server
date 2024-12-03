#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "config.h"
#include "client_manager.h"
#include "communication.h"

/**
 * Initialize and starts the server
 */
void * run_server() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return NULL;
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
        return NULL;
    }
    printf("Server is running on port %d\n", PORT);

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Socket listening failed");
        close(server_socket);
        return NULL;
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

            // recieve init message from the client
            char message[LOGIN_MESSAGE_SIZE] = {0};

            recv(client_socket, message, sizeof(message), 0);
            printf("Server receives message: %s\n", message);

            // check if the message is a login message
            if (strncmp(message, "LOGIN", 5) == 0) {
                printf("Login message received\n");
                // delimiter the message
                char *token = strtok(message, MESS_DELIMITER);
                token = strtok(NULL, MESS_END_CHAR);

                // Read the client's username
                char username[PLAYER_NAME_SIZE];
                strncpy(username, token, PLAYER_NAME_SIZE - 1);
                username[PLAYER_NAME_SIZE - 1] = '\0';

                // Handle the client in a separate thread
                pthread_t client_thread = 0;
                printf("Client username: %s\n", username);

                // Add the client to the client manager
                if (!add_client(client_socket, username, &client_thread)) {
                    perror("Failed to add client");
                    close(client_socket);
                    continue;
                }
                client *cl = get_client_by_socket(client_socket);

                // Create a new thread for the client
                if (pthread_create(&client_thread, NULL, run_client, cl) != 0) {
                    perror("Thread creation failed");
                    close(client_socket);
                    remove_client_by_socket(client_socket);
                    continue;
                }

//                char response[LOGIN_MESSAGE_RESP_SIZE] = {0};
//                sprintf(response, "LOGIN;%s;%s\n", cl->username, cl->client_char);
//                send_mess(cl, response);
                print_clients();
            } else {
                perror("Invalid message");
                close(client_socket);
            }
        }
    }
}


int main(void) {
    pthread_t server_thread, ping_thread;
    pthread_create(&server_thread, NULL, run_server, NULL);
    pthread_create(&ping_thread, NULL, run_ping, NULL);

    char input[1024];
    while (scanf("%s", input) != -1) {
        if (strcmp(input, "exit") == 0) {
            break;
        }
    }

    pthread_join(server_thread, NULL);
    pthread_join(ping_thread, NULL);
    pthread_cancel(server_thread);
    return 0;
}
