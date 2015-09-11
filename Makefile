CC = gcc
CFLAGS = -ggdb -pthread
INCLUDE = -I .

THREADING := self \
	equal

TIMING := rdtsc \
	gettimeofday	

TIMING_BIN := $(addprefix timing/, $(TIMING))
TIMING_SOURCES := $(addsuffix .c, $(TIMING_BIN))
THREADING_BIN := $(addprefix threading/, $(THREADING))
THREADING_SOURCES := $(addsuffix .c, $(THREADING_BIN))
CLEAN := $(TIMING) $(THREADING)

all: $(TIMING) $(THREADING)

%: timing/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

%: threading/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(CLEAN)
