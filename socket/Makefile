CC = gcc

CFLAGS = -Wall -Wextra -g -I../util
LDFLAGS = -lrt

TARGETS = server.out client.out

all: $(TARGETS)

SRCS = server.c client.c
OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build server executable
server.out: server.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build client executable
client.out: client.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGETS)

.PHONY: all clean

