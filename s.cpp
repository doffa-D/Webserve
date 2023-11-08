#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_PORTS 5

int main() {
    int server_socket[MAX_PORTS];
    struct sockaddr_in address[MAX_PORTS];
    int ports[MAX_PORTS] = {80, 8090, 8080, 8040, 8010};

    // Create sockets for each port
    for (int i = 0; i < MAX_PORTS; i++) {
        server_socket[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket[i] == -1) {
            std::cerr << "Failed to create socket for port " << ports[i] << std::endl;
            return 1;
        }

        // Initialize address structure
        address[i].sin_family = AF_INET;
        address[i].sin_addr.s_addr = INADDR_ANY;
        address[i].sin_port = htons(ports[i]);

        // Bind socket to the specified port
        if (bind(server_socket[i], (struct sockaddr *)&address[i], sizeof(address[i])) == -1) {
            std::cerr << "Failed to bind to port " << ports[i] << std::endl;
            return 1;
        }

        // Start listening
        if (listen(server_socket[i], 3) == -1) {
            std::cerr << "Failed to listen on port " << ports[i] << std::endl;
            return 1;
        }

        std::cout << "Server listening on port " << ports[i] << std::endl;
    }

    // Your other server logic here
    while(1);
    // Close all sockets
    for (int i = 0; i < MAX_PORTS; i++) {
        close(server_socket[i]);
    }

    return 0;
}
