#include "comm.h"

int make_conn()
{
    int fd = 0;
    struct sockaddr_in serv_addr;

    // Create socket fd
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(fd);
        return -1;
    }

    // Connect to server
    while (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("connecting to server...\n");
        sleep(2);
    }
    return fd;
}

void msg_proc(int fd)
{
    char buffer[BUFFER_SIZE] = {0};
    // Receive response from server
    // recv(fd, buffer, BUFFER_SIZE, 0);
    read(fd, buffer, BUFFER_SIZE);
    log("recv finish, Server: %s\n", buffer);
}

int main()
{
    char *message = "Hello from client";
    int fd = make_conn();

    // Send message to server
    // send(fd, message, strlen(message), 0);
    log("msg_send: %s", message);
    write(fd, message, strlen(message));
    log("send finish, will call receive");

    // proc msg received
    msg_proc(fd);

    // Clean up
    close(fd);
    return 0;
}

