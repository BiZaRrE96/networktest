#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_in server_addr, client_addr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Fill server information
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }

    std::cout << "Receiver ready. Listening on port " << PORT << "...\n";

    socklen_t len = sizeof(client_addr);
    while (true) {
        memset(buffer, 0, BUF_SIZE);
        int n = recvfrom(sockfd, buffer, BUF_SIZE, MSG_WAITALL,
                         (struct sockaddr *)&client_addr, &len);
        if (n < 0) {
            perror("Receive failed");
            break;
        }

        std::cout << "Received message: " << buffer << "\n";
    }

    close(sockfd);
    return 0;
}
