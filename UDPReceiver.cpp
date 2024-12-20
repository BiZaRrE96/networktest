#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define RECV_PORT 8080
#define SEND_PORT 6969
#define BUF_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_in recv_addr, send_addr, client_addr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Fill receiver information
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = INADDR_ANY;
    recv_addr.sin_port = htons(RECV_PORT);

    // Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }

    std::cout << "Receiver ready. Listening on port " << RECV_PORT << "...\n";

    // Fill sender information
    memset(&send_addr, 0, sizeof(send_addr));
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(SEND_PORT);
    send_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

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

        // Respond back
        std::string reply = "Acknowledged: " + std::string(buffer);
        if (sendto(sockfd, reply.c_str(), reply.length(), MSG_CONFIRM,
                   (const struct sockaddr *)&send_addr, sizeof(send_addr)) < 0) {
            perror("Send failed");
            break;
        }
    }

    close(sockfd);
    return 0;
}
