# `bear make` can generate compile_commands.json for clangd

TARGETS = server.out client.out

CFLAGS = -MMD -MP -Wall -Wextra -g -I../util
LDFLAGS = -lrt

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
-include $(DEPS)

.PHONY: all
all: $(TARGETS)

# Build server executable
server.out: server.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build client executable
client.out: client.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGETS) $(DEPS)
