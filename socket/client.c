#include "comm.h"

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *message = "Hello from client";
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(sock);
        return -1;
    }

    // Connect to server
    while (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("connecting to server...\n");
        sleep(2);
    }

    // Send message to server
    // send(sock, message, strlen(message), 0);
    write(sock, message, strlen(message));
    printf("Message sent\n");

    // Receive response from server
    // recv(sock, buffer, BUFFER_SIZE, 0);
    read(sock, buffer, BUFFER_SIZE);
    printf("Server: %s\n", buffer);

    // Clean up
    close(sock);

    return 0;
}

