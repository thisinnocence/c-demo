#include "comm.h"

void make_conn(int *pserver_fd, int *pconn_fd)
{
    int server_fd, conn_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 设置 SO_REUSEADDR 选项
    int optval = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Accept a connection
    if ((conn_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    *pserver_fd = server_fd;
    *pconn_fd = conn_fd;
}

int main()
{
    int server_fd, conn_fd;
    make_conn(&server_fd, &conn_fd);

    char buffer[BUFFER_SIZE] = {0};
    const char *response = "Hello from server";
    // Read data from client
    read(conn_fd, buffer, BUFFER_SIZE);
    log("recv client req: %s", buffer);

    // Send response to client
    send(conn_fd, response, strlen(response), 0);
    log("response sent");

    // Clean up
    close(conn_fd);
    close(server_fd);
    return 0;
}

