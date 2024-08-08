TARGETS = sc.out event.out async_event.out

.PHONY: all
all: $(TARGETS)

SYSTEMC_HOME = /root/github/systemc
INC_DIRS = ../util $(SYSTEMC_HOME)/include $(SYSTEMC_HOME)/examples/sysc/async_suspend
INC_FLAGS = $(addprefix -I, $(INC_DIRS))

CXXFLAGS = $(INC_FLAGS) -MMD -MP -std=c++17 -Wall -g
LDFLAGS = -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc -Wl,-rpath,$(SYSTEMC_HOME)/lib-linux64 \
			-lpthread

SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)
DEPS = $(SRCS:.cc=.d)
-include $(DEPS)

sc.out: sc.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

event.out: event.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

async_event.out: async_event.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o *.d *.out
