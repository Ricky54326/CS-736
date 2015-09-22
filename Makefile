CC := gcc
CFLAGS := -pthread -ggdb
INCLUDE := -I .

CFLAGS := $(CFLAGS) $(BUILD_FLAGS)

THREADING := self \
	equal \
	time_pthread \
	time_proc \
	fault

TIMING := rdtsc \
	gettimeofday

CONTEXT := signal \
	pipe \
	mutex \
	cond

CONTEXT_BIN := $(addprefix context-switch/, $(CONTEXT))
CONTEXT_SOURCES := $(addsuffix .c, $(CONTEXT_BIN))
TIMING_BIN := $(addprefix timing/, $(TIMING))
TIMING_SOURCES := $(addsuffix .c, $(TIMING_BIN))
THREADING_BIN := $(addprefix threading/, $(THREADING))
THREADING_SOURCES := $(addsuffix .c, $(THREADING_BIN))
CLEAN := $(TIMING) $(THREADING) $(CONTEXT) child.txt \
	parent.txt \
	calc \
	output.txt \
	fastest.txt
	

all: $(TIMING) $(THREADING) $(CONTEXT) calc

%: timing/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

%: threading/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

%: context-switch/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@
calc:
	$(CC) $(CFLAGS) $(INCLUDE) calc.c -o calc

clean:
	rm -rf $(CLEAN)
