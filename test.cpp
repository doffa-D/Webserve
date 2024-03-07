#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1048576 // 1 MB (adjust as needed)

int main() {
    // Create a socket (you need to replace this with actual socket creation code)
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Connect to a server (you need to replace this with actual connection code)
    // ...

    // Experiment with different buffer sizes
    for (int buffer_size = 436207616; buffer_size <= MAX_BUFFER_SIZE; buffer_size *= 2) {
        char* buffer = new char[buffer_size];
        ssize_t bytes_received = recv(socket_fd, buffer, buffer_size, 0);

        if (bytes_received == -1) {
            std::cerr << "Error receiving data with buffer size: " << buffer_size << std::endl;
            delete[] buffer;
            break;
        }

        // Process received data (if needed)
        // ...

        delete[] buffer;
    }

    // Close the socket
    close(socket_fd);

    return 0;
}
