#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT1 8080
#define PORT2 8081
#define SERVER_IP "10.11.3.5" // Changed IP address to the one provided
#define BUFFER_SIZE 1024

int send_request(int port)
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation failed");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // SERVER_IP is now set to the desired address
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
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

#include <sys/wait.h>

int main()
{
    int max_processes = 100; // Maximum number of concurrent child processes
    int active_processes = 0;

    for (int i = 0; i < 50000; ++i)
    {
        if (active_processes >= max_processes)
        {
            // Wait for any child process to finish
            wait(NULL);
            --active_processes;
        }

        int dd = fork();
        if (dd == 0)
        {
            send_request(9090);
            return 0; // Child process ends after sending request
        }
        else if (dd > 0)
        {
            ++active_processes; // Increase the count of active child processes
        }
    }

    // Wait for all child processes to finish
    while (active_processes > 0)
    {
        wait(NULL);
        --active_processes;
    }

    return 0;
}
