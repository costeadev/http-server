#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 6767

int main(void) {

    char server_message[] = "You have reached the server";
    char unknown_message[] = "Yokese tio";
    char *cats = "https://encrypted-tbn0.gstatic.com/"
                 "images?q=tbn:ANd9GcSR7aZFMLRXe6MC-O1hnuOWEkpH7la9Fts9O2AnO-6sDQ&s";

    int server_socket;
    int client_socket;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        printf("Creating the socket failed (%d)\n\n", server_socket);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("Binding the socket failed!\n\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0) {
        printf("Socket listening failed\n\n");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);
    fflush(stdout);

    while (1) {
        client_socket = accept(server_socket, NULL, NULL);

        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        printf("Client connected\n");
        send(client_socket, server_message, strlen(server_message), 0);

        char buffer[256];
        ssize_t bytes_received;

        while ((bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[bytes_received] = '\0';
            buffer[strcspn(buffer, "\r\n")] = '\0';

            printf("Received (%zd bytes): %s\n", bytes_received, buffer);

            if (strcmp(buffer, "GET /api/cats") == 0) {
                send(client_socket, cats, strlen(cats), 0);
            } else {
                send(client_socket, unknown_message, strlen(unknown_message), 0);
            }
        }

        printf("Client disconnected\n");
        close(client_socket);
    }

    close(server_socket);

    return 0;
}