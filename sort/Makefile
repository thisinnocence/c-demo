SRCS = $(wildcard *.c)
TARGETS = $(SOURCES:.cc=.out)

.PHONY: all
all: $(TARGETS)

CFLAGS  = -MMD -MP -Wall -g

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
-include $(DEPS)

%.out: %.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o *.d *.out
