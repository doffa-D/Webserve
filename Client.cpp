#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int sendRequest()
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    std::string message = "GET / HTTP/1.1\r\nHost: 10.11.1.3:8080\r\nConnection: close\r\n\r\n";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if(inet_pton(AF_INET, "10.11.1.3", &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }

    send(sock, message.c_str(), message.length(), 0);
    while ((valread = read(sock, buffer, 1024)) > 0) {
        std::cout << buffer << std::endl;
    }
    return 0;
}

int main() 
{
    // fork();
    // for (int i = 0; i < 6740; i++)
    // {
        sendRequest();
    // }
    // wait(NULL);
    return 0;
}



