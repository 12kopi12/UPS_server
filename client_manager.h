#ifndef __CLIENT_MANAGER_H__
#define __CLIENT_MANAGER_H__

#include <pthread.h>
#include "config.h"

/**
 * Add new client to the array of clients
 * @param socket client's socket
 * @param username client's username
 * @param thread client's thread
 * @return TRUE if client was added, FALSE if client already exists
 */
int add_client(int socket, char *username, pthread_t *thread);

/**
 * Remove client
 * @param cl client to remove
 * @return TRUE if client was removed, FALSE if not found
 */
int remove_client(client *cl);

/**
 * Remove client by his socket
 * @param socket socket of the client
 * @return TRUE if client was removed, FALSE if not found
 */
int remove_client_by_socket(int socket);

/**
 * Get client by his socket
 * @param socket socket of the client
 * @return client or NULL if not found
 */
client *get_client_by_socket(int socket);

/**
 * Get count of connected clients
 * @return count of connected clients
 */
int get_connected_clients_count();

#endif