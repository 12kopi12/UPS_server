#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include "config.h"

/**
 * @brief Send message to the client
 * @return
 */
void *send_mess(client *client, char *mess);

/**
 * @brief Recieve message from the client
 * @param arg void pointer (client *)
 * @return void pointer
 */
void *receive_messages(void *arg);

#endif