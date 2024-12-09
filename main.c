//#include <stdio.h>
//#include <pthread.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <unistd.h>
//#include <string.h>
//#include <stdlib.h>
//
//#include "config.h"
//#include "client_manager.h"
//#include "communication.h"
//
//server_address server_info;
//
///**
// * Initialize and starts the server
// */
//void * run_server() {
//    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
//    if (server_socket == -1) {
//        perror("Socket creation failed");
//        return NULL;
//    }
//
//    // Set socket options for reusing the address
//    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
//
//    // Set the server address and port
//    struct sockaddr_in server_address;
//    memset(&server_address, 0, sizeof(server_address));
//    server_address.sin_family = AF_INET;
////    // todo sem dostat volitelny port
////    server_address.sin_port = htons(PORT);
////    // todo sem dostat volitelnou ip adresu
//    server_address.sin_addr.s_addr = INADDR_ANY;
//    server_address.sin_port = htons(server_info.port);
////    server_address.sin_addr.s_addr = inet_addr(server_info.ip_address);
////    if (inet_pton(AF_INET, server_info.ip_address, &server_address.sin_addr) <= 0) {
////        perror("Invalid IP address");
////        close(server_socket);
////        return NULL;
////    }
////    printf("Server IP: %s\n", server_info.ip_address);
//
//    // Bind the socket to the address and port
//    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
//        perror("Socket binding failed");
//        close(server_socket);
//        return NULL;
//    }
//    printf("Server is running on port %d\n", server_info.port);
//
//    // Listen for incoming connections
//    if (listen(server_socket, MAX_CLIENTS) == -1) {
//        perror("Socket listening failed");
//        close(server_socket);
//        return NULL;
//    }
//
//    // Accept incoming connections
//    struct sockaddr_in client_address;
//    socklen_t client_address_len = sizeof(client_address);
//    while (1) {
//        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
//        if (client_socket == -1) {
//            perror("Socket accepting failed");
//        } else {
//            printf("Client connected\n");
//
//            // recieve init message from the client
//            char message[LOGIN_MESSAGE_SIZE] = {0};
//
//            recv(client_socket, message, sizeof(message), 0);
//            printf("Server receives message: %s\n", message);
//
//            char *token = strtok(message, MESS_DELIMITER);
//            // check if the message is a login message
////            if (strncmp(message, "LOGIN", 5) == 0) {
//            if (strcmp(token, "LOGIN") == 0) {
//                printf("Login message received\n");
//                // delimiter the message
////               todo char *token = strtok(message, MESS_DELIMITER);
//                token = strtok(NULL, MESS_END_CHAR);
//                if (token == NULL) {
//                    perror("Invalid message");
//                    close(client_socket);
//                    continue;
//                }
//
//                // Read the client's username
//                char username[PLAYER_NAME_SIZE];
//                strncpy(username, token, PLAYER_NAME_SIZE - 1);
//                username[PLAYER_NAME_SIZE - 1] = '\0';
//
//                // Handle the client in a separate thread
//                pthread_t client_thread = 0;
//                printf("Client username: %s\n", username);
//
//                // Add the client to the client manager
//                if (!add_client(client_socket, username, &client_thread)) {
//                    perror("Failed to add client");
//                    close(client_socket);
//                    continue;
//                }
//                client *cl = get_client_by_socket(client_socket);
//
//                // Create a new thread for the client
//                if (pthread_create(&client_thread, NULL, run_client, cl) != 0) {
//                    perror("Thread creation failed");
//                    close(client_socket);
//                    remove_client_by_socket(client_socket);
//                    continue;
//                }
//
////                char response[LOGIN_MESSAGE_RESP_SIZE] = {0};
////                sprintf(response, "LOGIN;%s;%s\n", cl->username, cl->client_char);
////                send_mess(cl, response);
//                print_clients();
//            } else {
//                perror("Invalid connection message");
//                close(client_socket);
//            }
//        }
//    }
//}
//
//
//int main(int argc, char *argv[]) {
//    // Set the server address and port
////    strcpy(server_info.ip_address, "172.31.210.166");
//    server_info.port = PORT;
////
////    if (argc > 1) {
////        strncpy(server_info.ip_address, "\0", sizeof(server_info.ip_address));
////        strncpy(server_info.ip_address, argv[1], sizeof(server_info.ip_address) - 1);
////        server_info.ip_address[sizeof(server_info.ip_address) - 1] = '\0';
////    }
////    if (argc > 2) {
////        server_info.port = atoi(argv[2]);
////        if (server_info.port <= 0 || server_info.port > 65535) {
////            fprintf(stderr, "Invalid port number. Please provide a port between 1 and 65535.\n");
////            return 1;
////        }
////    }
//
//    pthread_t server_thread, ping_thread;
//    pthread_create(&server_thread, NULL, run_server, NULL);
//    pthread_create(&ping_thread, NULL, run_ping, NULL);
//
////    char input[1024];
////    while (scanf("%s", input) != -1) {
////        if (strcmp(input, "exit") == 0) {
////            break;
////        }
////    }
//
//    pthread_join(server_thread, NULL);
//    pthread_join(ping_thread, NULL);
//    pthread_cancel(server_thread);
//    return 0;
//}
//
//








#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "client_manager.h"
#include "communication.h"

server_address server_info;

/**
 * @brief Validuje IP adresu.
 * @param ip_address IP adresa k validaci.
 * @return 1 pokud je IP adresa platná, jinak 0.
 */
int validate_ip(const char *ip_address) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip_address, &(sa.sin_addr)) != 0;
}

/**
 * @brief Nastavuje serverové informace.
 * @param argc Počet argumentů příkazové řádky.
 * @param argv Argumenty příkazové řádky.
 */
void set_server_info(int argc, char *argv[]) {
    // Set the default server address and port
//    strcpy(server_info.ip_address, "127.0.0.1");
    strcpy(server_info.ip_address, "");
    server_info.port = PORT;

    // Program arguments
    if (argc > 1) {
        if (validate_ip(argv[1])) {
            strncpy(server_info.ip_address, argv[1], sizeof(server_info.ip_address) - 1);
            server_info.ip_address[sizeof(server_info.ip_address) - 1] = '\0';
        } else {
            fprintf(stderr, "Invalid IP address: %s\n", argv[1]);
            exit(1);
        }
    }
    if (argc > 2) {
        server_info.port = atoi(argv[2]);
        if (server_info.port <= 0 || server_info.port > 65535) {
            fprintf(stderr, "Invalid port: %s. Input number in range: 1-65535.\n", argv[2]);
            exit(1);
        }
    }
}

/**
 * Inicializace a spuštění serveru
 */
void *run_server() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_info.port);

    if (strlen(server_info.ip_address) == 0) {
        server_address.sin_addr.s_addr = INADDR_ANY;
        printf("Server is running at INADDR_ANY (all available addresses)\n");
    } else {
        server_address.sin_addr.s_addr = inet_addr(server_info.ip_address);
        printf("Server IP: %s\n", server_info.ip_address);
    }

//    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

//    if (inet_pton(AF_INET, server_info.ip_address, &server_address.sin_addr) <= 0) {
//        perror("Invalid IP address");
//        close(server_socket);
//        exit(1);
//    }

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Socket binding failed");
        close(server_socket);
        exit(1);
    }
    printf("Server is running on port %d\n", server_info.port);

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Socket listening failed");
        close(server_socket);
        return NULL;
    }

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    while (1) {
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket == -1) {
            perror("Socket accepting failed");
        } else {
            printf("Client connected\n");
// recieve init message from the client
            char message[LOGIN_MESSAGE_SIZE] = {0};

            recv(client_socket, message, sizeof(message), 0);

            char *token = strtok(message, MESS_DELIMITER);
            // check if the message is a login message
//            if (strncmp(message, "LOGIN", 5) == 0) {
            if (strcmp(token, "LOGIN") == 0) {
                printf("Login message received\n");
                // delimiter the message
                token = strtok(NULL, MESS_END_CHAR);
                if (token == NULL) {
                    perror("Invalid message -> close client socket");
                    close(client_socket);
                    continue;
                }

                // Read the client's username
                char username[PLAYER_NAME_SIZE];
                strncpy(username, token, PLAYER_NAME_SIZE - 1);
                username[PLAYER_NAME_SIZE - 1] = '\0';

                // Handle the client in a separate thread
                pthread_t client_thread = 0;
//                printf("Client username: %s\n", username);

                // Add the client to the client manager
                if (!add_client(client_socket, username, &client_thread)) {
                    perror("Failed to add client -> close client socket");
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
                perror("Invalid connection message -> close client socket");
                close(client_socket);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    set_server_info(argc, argv);

    pthread_t server_thread, ping_thread;
    pthread_create(&server_thread, NULL, run_server, NULL);
    pthread_create(&ping_thread, NULL, run_ping, NULL);

    pthread_join(server_thread, NULL);
    pthread_join(ping_thread, NULL);
    pthread_cancel(server_thread);
    return 0;
}

