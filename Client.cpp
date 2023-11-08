#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_PORTS 5
#define REQUEST "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"
#define REQUEST_LEN strlen(REQUEST)

void* send_request(void* arg) {
    int port = *((int*)arg);
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return NULL;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        close(sockfd);
        return NULL;
    }

    // Send the request
    if (send(sockfd, REQUEST, REQUEST_LEN, 0) < 0) {
        perror("Error sending request");
    } else {
        printf("Request sent to port %d\n", port);
    }

    close(sockfd);
    return NULL;
}

int main() {
    int ports[MAX_PORTS] = {80, 8090, 8080, 8040, 8010};
    pthread_t threads[MAX_PORTS];

    for (int i = 0; i < MAX_PORTS; i++) {
        pthread_create(&threads[i], NULL, send_request, &ports[i]);
    }

    for (int i = 0; i < MAX_PORTS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
