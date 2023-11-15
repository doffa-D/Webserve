#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT1 8080
#define PORT2 8081
#define SERVER_IP "10.11.1.3"  // Changed IP address to the one provided
#define BUFFER_SIZE 1024

int send_request(int port) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // SERVER_IP is now set to the desired address
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return 1;
    }

    char message[] = "Hello from client!";
    send(sock, message, strlen(message), 0);
    printf("Request sent to port %d\n", port);

    char buffer[BUFFER_SIZE] = {0};
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Response from port %d: %s\n", port, buffer);

    close(sock);
    return 0;
}

int main() {
    for (int i = 0; i < 500; ++i) {
        if(send_request(8081) == 1)
            continue;
        // if(send_request(8080) == 1)
        //     continue;
    }

    return 0;
}
