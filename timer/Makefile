CC = gcc

CFLAGS = -Wall -Wextra -g -I../util
LDFLAGS = -lrt  # timer库依赖rt这个

TARGETS = timer.out

all: $(TARGETS)

SRCS = timer.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

-include $(DEPS)

%.o: %.c %.d
	$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c
	@set -e; \
	$(CC) $(CFLAGS) -MM $< | \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

timer.out: timer.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGETS) $(DEPS)

