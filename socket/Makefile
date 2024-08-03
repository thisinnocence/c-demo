# auto-gen by chat-gpt
#
# `bear make` can generate compile_commands.json
# is used for clangd-lsp for vim/vscode


CC = gcc

CFLAGS = -Wall -Wextra -g -I../util
LDFLAGS = -lrt

TARGETS = server.out client.out

all: $(TARGETS)

SRCS = server.c client.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

-include $(DEPS)

%.o: %.c %.d
	$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c
	@set -e; \
	$(CC) $(CFLAGS) -MM $< | \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

# Build server executable
server.out: server.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build client executable
client.out: client.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGETS) $(DEPS)

.PHONY: all clean
