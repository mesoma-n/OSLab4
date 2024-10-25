#include <netinet/in.h> // structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // for socket APIs
#include <sys/types.h>
#include <unistd.h> // for close()
#include "list.h"

#define PORT 9001
#define ACK "ACK"

int main(int argc, char const* argv[]) {
    int n, val, idx;
    // create server socket
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);

    if (servSockD == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // string store data to recv/send to/from client
    char buf[1024];
    char sbuf[1024];
    char* token;

    // define server address
    struct sockaddr_in servAddr;

    // linked list
    list_t *mylist;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    // bind socket to the specified IP and port
    if (bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Failed to bind socket");
        close(servSockD);
        exit(EXIT_FAILURE);
    }

    // listen for connections
    if (listen(servSockD, 1) < 0) {
        perror("Failed to listen on socket");
        close(servSockD);
        exit(EXIT_FAILURE);
    }

    // integer to hold client socket
    int clientSocket = accept(servSockD, NULL, NULL);
    if (clientSocket < 0) {
        perror("Failed to accept client connection");
        close(servSockD);
        exit(EXIT_FAILURE);
    }

    mylist = list_alloc();  // create the list

    while (1) {
        // receive messages from client socket
        n = recv(clientSocket, buf, sizeof(buf), 0);
        if (n <= 0) {
            perror("Failed to receive data or client disconnected");
            break;
        }
        buf[n] = '\0';

        token = strtok(buf, " ");

        if (strcmp(token, "exit") == 0) {
            list_free(mylist);
            sprintf(sbuf, "Server shutting down...");
            send(clientSocket, sbuf, sizeof(sbuf), 0);
            break;
        } else if (strcmp(token, "get_length") == 0) {
            val = list_length(mylist);
            sprintf(sbuf, "Length = %d", val);
        } else if (strcmp(token, "add_front") == 0) {
            token = strtok(NULL, " ");  // get next token (value)
            val = atoi(token);
            list_add_to_front(mylist, val);
            sprintf(sbuf, "%s %d", ACK, val);
        } else if (strcmp(token, "add_back") == 0) {
            token = strtok(NULL, " ");
            val = atoi(token);
            list_add_to_back(mylist, val);
            sprintf(sbuf, "%s %d", ACK, val);
        } else if (strcmp(token, "add_position") == 0) {
            token = strtok(NULL, " ");
            idx = atoi(token);
            token = strtok(NULL, " ");
            val = atoi(token);
            list_add_at_index(mylist, idx, val);
            sprintf(sbuf, "%s %d at %d", ACK, val, idx);
        } else if (strcmp(token, "remove_back") == 0) {
            val = list_remove_from_back(mylist);
            sprintf(sbuf, "Removed = %d", val);
        } else if (strcmp(token, "remove_front") == 0) {
            val = list_remove_from_front(mylist);
            sprintf(sbuf, "Removed = %d", val);
        } else if (strcmp(token, "remove_position") == 0) {
            token = strtok(NULL, " ");
            idx = atoi(token);
            val = list_remove_at_index(mylist, idx);
            sprintf(sbuf, "Removed = %d from position %d", val, idx);
        } else if (strcmp(token, "get") == 0) {
            token = strtok(NULL, " ");
            idx = atoi(token);
            val = list_get_elem_at(mylist, idx);
            sprintf(sbuf, "Element at %d = %d", idx, val);
        } else if (strcmp(token, "print") == 0) {
            FILE *fp = fopen("/tmp/list_output.txt", "w+");
            if (fp != NULL) {
                int stdout_backup = dup(STDOUT_FILENO);
                dup2(fileno(fp), STDOUT_FILENO);
                list_print(mylist);
                fflush(fp);
                dup2(stdout_backup, STDOUT_FILENO);
                fclose(fp);

                fp = fopen("/tmp/list_output.txt", "r");
                fread(sbuf, 1, sizeof(sbuf), fp);
                fclose(fp);
            } else {
                sprintf(sbuf, "Failed to print list");
            }
        } else {
            sprintf(sbuf, "Invalid command");
        }

        // send message to client socket
        send(clientSocket, sbuf, sizeof(sbuf), 0);
        memset(buf, '\0', 1024);
    }

    list_free(mylist);
    close(clientSocket);
    close(servSockD);
    return 0;
}
