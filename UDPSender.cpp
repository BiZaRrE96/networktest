#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define SEND_PORT 6969
#define RECV_PORT 8080
#define BUF_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_in recv_addr, send_addr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Fill sender information
    memset(&send_addr, 0, sizeof(send_addr));
    send_addr.sin_family = AF_INET;
    send_addr.sin_addr.s_addr = INADDR_ANY;
    send_addr.sin_port = htons(SEND_PORT);

    // Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&send_addr, sizeof(send_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }

    std::cout << "Sender ready. Bound to port " << SEND_PORT << ".\n";

    // Fill receiver information
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(RECV_PORT);
    recv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (true) {
        // Send message
        std::cout << "Enter message to send: ";
        std::cin.getline(buffer, BUF_SIZE);

        if (sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM,
                   (const struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0) {
            perror("Send failed");
            break;
        }

        std::cout << "Message sent: " << buffer << "\n";

        // Receive response
        memset(buffer, 0, BUF_SIZE);
        socklen_t len = sizeof(recv_addr);
        int n = recvfrom(sockfd, buffer, BUF_SIZE, MSG_WAITALL,
                         (struct sockaddr *)&recv_addr, &len);
        if (n < 0) {
            perror("Receive failed");
            break;
        }

        std::cout << "Received response: " << buffer << "\n";
    }

    close(sockfd);
    return 0;
}
