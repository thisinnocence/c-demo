#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>

#include "log.h"

#define PORT 8080
#define CONN_IP "127.0.0.1"
#define BUFFER_SIZE 1024

enum msg_type {
    Client = 1,
    Server,
};

// 定义单字节对齐的宏
#define PACKED_STRUCT(name) \
    struct __attribute__((packed, aligned(1))) name

PACKED_STRUCT(header) {
    uint16_t type;
    uint16_t length;
};

#endif
