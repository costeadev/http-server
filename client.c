#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 6767

int main(void) {
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (network_socket < 0) {
        printf("Creating the socket failed (%d)\n\n", network_socket);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status =
        connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    if (connection_status < 0) {
        printf("There was an error making a connection to the server (%d)\n\n", connection_status);
        exit(EXIT_FAILURE);
    }

    char server_response[256];
    ssize_t n = recv(network_socket, &server_response, sizeof(server_response), 0);
    if (n > 0) {
        server_response[n] = '\0';
        printf("The server sent: %s\n", server_response);
    }

    char buffer[256];

    while (1) {
        fgets(buffer, sizeof(buffer), stdin);

        send(network_socket, buffer, strlen(buffer), 0);

        char recv_buf[256];
        ssize_t recv_bytes = recv(network_socket, recv_buf, sizeof(recv_buf), 0);
        if (recv_bytes < 0) {
            perror("recv");
            continue;
        }

        char *recv_str = strndup(recv_buf, recv_bytes);
        printf("Received from server: %s\n\n", recv_str);
    }

    close(network_socket);

    return 0;
}