#include "comm.h"

int make_conn()
{
    int fd = 0, ret = 0;
    struct sockaddr_in serv_addr;

    /* Create socket fd:
       SOCK_STREAM: Sequenced, reliable, connection-based byte streams.
       这个表示有序可靠基于连接，协议可以用TCP，也可以是本机domain-socket. */
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    /* Configure server address:
       这个是表示协议类型，IP协议栈而且是stream, 基本就是TCP了.
       AF_INET == PF_INET == 2 , means IP protocol family. */
    serv_addr.sin_family = AF_INET;   
    serv_addr.sin_port = htons(PORT);

    /* Convert IPv4 and IPv6 addresses from text to binary form */
    if (inet_pton(AF_INET, CONN_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(fd);
        return -1;
    }

    /* Connect to server */
CONN:
    ret = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret < 0) {
        if (errno == ECONNREFUSED) {
            static int retry_cnt = 0;
            log("Connection refused: no server listening at the port %d, retry [%02d]...", PORT, ++retry_cnt);
            sleep(1);
            goto CONN;
        } else {
            perror("Connect error");
            return -1;
        }
    }

    log("connects to server OK!");
    return fd;
}

void msg_send(int fd)
{
    struct header hdr = {0};
    char *message = "Hello from client";
    hdr.type = Client;
    hdr.length = strlen(message);

    log("send header: type %u len %u", hdr.type, hdr.length);
    write(fd, &hdr, sizeof(hdr));
    log("send body: %s", message);
    // send(fd, message, strlen(message), 0);
    write(fd, message, strlen(message));
}

void msg_recv(int fd)
{
    char buffer[BUFFER_SIZE] = {0};
    // recv(fd, buffer, BUFFER_SIZE, 0);
    read(fd, buffer, BUFFER_SIZE);
    log("recv finish, Server: %s", buffer);
}

int main()
{
    int fd = make_conn();
    if (fd < 0) {
        log("connect error!");
        exit(1);
    }

    msg_send(fd);
    msg_recv(fd);

    close(fd);
    return 0;
}

