#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_PORTS 5
#define MAX_REQUEST_SIZE 1024

int main() {
    int ports[MAX_PORTS] = {80, 8090, 8080, 8040, 8010};
    int sockfd[MAX_PORTS];
    fd_set readfds;

    // Create and bind sockets for each port
    for (int i = 0; i < MAX_PORTS; i++) {
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd[i] < 0) {
            perror("Error creating socket");
            exit(1);
        }

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(ports[i]);

        if (bind(sockfd[i], (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Error binding socket");
            exit(1);
        }

        if (listen(sockfd[i], 5) < 0) {
            perror("Error listening on socket");
            exit(1);
        }
    }

    printf("Listening on ports 80, 8090, 8080, 8040, 8010...\n");

    while (1) {
        FD_ZERO(&readfds);

        // Add all socket file descriptors to the set
        for (int i = 0; i < MAX_PORTS; i++) {
            FD_SET(sockfd[i], &readfds);
        }

        // Use select to monitor sockets for incoming connections
        int max_fd = -1;
        for (int i = 0; i < MAX_PORTS; i++) {
            if (sockfd[i] > max_fd) {
                max_fd = sockfd[i];
            }
        }

        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("Error in select");
            exit(1);
        }

        // Check which socket has incoming data
        for (int i = 0; i < MAX_PORTS; i++) {
            if (FD_ISSET(sockfd[i], &readfds)) {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_sock = accept(sockfd[i], (struct sockaddr *)&client_addr, &client_len);
                if (client_sock < 0) {
                    perror("Error accepting connection");
                    continue;
                }

                // Read and print the request from the client
                char request[MAX_REQUEST_SIZE];
                ssize_t bytes_received = recv(client_sock, request, sizeof(request), 0);
                if (bytes_received < 0) {
                    perror("Error reading from socket");
                } else if (bytes_received == 0) {
                    printf("Client disconnected\n");
                } else {
                    request[bytes_received] = '\0';
                    printf("Received request from client on port %d:\n%s\n", ports[i], request);
                }

                // Close the client socket
                close(client_sock);
            }
        }
    }

    // Close all sockets (this part of the code is unreachable in this example)
    for (int i = 0; i < MAX_PORTS; i++) {
        close(sockfd[i]);
    }

    return 0;
}
