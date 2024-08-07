TARGETS = sc.out

.PHONY: all
all: $(TARGETS)

SYSTEMC_HOME = /root/github/systemc
INC_DIRS = ../util $(SYSTEMC_HOME)/include
INC_FLAGS = $(addprefix -I, $(INC_DIRS))

CXXFLAGS = $(INC_FLAGS) -MMD -MP -std=c++17 -Wall -g
LDFLAGS = -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc -Wl,-rpath,$(SYSTEMC_HOME)/lib-linux64

SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)
DEPS = $(SRCS:.cc=.d)
-include $(DEPS)

sc.out: sc.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGETS) $(DEPS)
