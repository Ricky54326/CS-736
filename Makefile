CC = gcc
CFLAGS = -ggdb
INCLUDE =

TIMING := rdtsc \
	gettimeofday	

TIMING_BIN := $(addprefix timing/, $(TIMING))
TIMING_SOURCES := $(addsuffix .c, $(TIMING_BIN))

CLEAN := $(TIMING)

all: $(TIMING)

%: timing/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(CLEAN)
