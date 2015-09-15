CC = gcc
CFLAGS = -ggdb -pthread
INCLUDE = -I .

THREADING := self \
	equal \
	time_pthread \
	time_proc

TIMING := rdtsc \
	gettimeofday

CONTEXT := signal \
	pipe

CONTEXT_BIN := $(addprefix context-switch/, $(CONTEXT))
CONTEXT_SOURCES := $(addsuffix .c, $(CONTEXT_BIN))
TIMING_BIN := $(addprefix timing/, $(TIMING))
TIMING_SOURCES := $(addsuffix .c, $(TIMING_BIN))
THREADING_BIN := $(addprefix threading/, $(THREADING))
THREADING_SOURCES := $(addsuffix .c, $(THREADING_BIN))
CLEAN := $(TIMING) $(THREADING) $(CONTEXT)

all: $(TIMING) $(THREADING) $(CONTEXT)

%: timing/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

%: threading/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

%: context-switch/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(CLEAN)
