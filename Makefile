GCC=gcc
#GCC=riscv64-unknown-linux-gnu-gcc
CFLAGS=-O0 -DON_MULTIPLE_LINES=1 -DMMAP_ARRAY
#CFLAGS=-O0
INCLUDE=-Iinclude/
EVICT_SRC=src/evict.c src/eviction_evaluation.c
HIST_SRC=src/evict.c src/timing_histogram.c
BENCH_SRC=src/generate_compilation_commands.c


evict:
	mkdir -p eviction_output
	$(GCC) $(INCLUDE) $(CFLAGS) $(EVICT_SRC) -o eviction_histogram

evict-unmatched: TARGET=-DBOARD=UNMATCHED
evict-unmatched:
	mkdir -p eviction_output
	$(GCC) $(INCLUDE) $(CFLAGS) $(EVICT_SRC) $(TARGET) -o eviction_histogram

# template
histogram-template: TARGET=-DBOARD=
	mkdir -p histogram_output
	$(GCC) $(INCLUDE) $(CFLAGS) $(HIST_SRC) $(TARGET) -o timing_histogram

histogram-unmatched: TARGET=-DBOARD=UNMATCHED
histogram-unmatched:
	mkdir -p histogram_output
	$(GCC) $(INCLUDE) $(CFLAGS) $(HIST_SRC) $(TARGET) -o timing_histogram

histogram-ahead: TARGET=-DBOARD=BV_AHEAD
histogram-ahead:
	mkdir -p histogram_output
	$(GCC) $(INCLUDE) $(CFLAGS) $(HIST_SRC) $(TARGET) -o timing_histogram

histogram-fire: TARGET=-DBOARD=BV_FIRE
histogram-fire:
	mkdir -p histogram_output
	$(GCC) $(INCLUDE) $(CFLAGS) $(HIST_SRC) $(TARGET) -o timing_histogram


# template
benchmark-template: TARGET=
	mkdir -p benchmark_output
	$(GCC) $(INCLUDE) $(CFLAGS) $(BENCH_SRC) -DBOARD=$(TARGET) -o generate_compilation_commands_$(TARGET)
	./generate_compilation_commands_$(TARGET) -m $(TARGET)

benchmark-unmatched: TARGET=UNMATCHED
benchmark-unmatched:
	mkdir -p benchmark_output
	$(GCC) $(INCLUDE) $(CFLAGS) $(BENCH_SRC) -DBOARD=$(TARGET) -o generate_compilation_commands_$(TARGET)
	./generate_compilation_commands_$(TARGET) -m $(TARGET)

benchmark-ahead: TARGET=BV_AHEAD
benchmark-ahead:
	mkdir -p benchmark_output
	$(GCC) $(INCLUDE) $(CFLAGS) $(BENCH_SRC) -DBOARD=$(TARGET) -o generate_compilation_commands_$(TARGET)
	#./generate_compilation_commands_$(TARGET) -m $(TARGET)

benchmark-fire: TARGET=BV_FIRE
benchmark-fire:
	mkdir -p benchmark_output
	$(GCC) $(INCLUDE) $(CFLAGS) $(BENCH_SRC) -DBOARD=$(TARGET) -o generate_compilation_commands_$(TARGET)
	#./generate_compilation_commands_$(TARGET) -m $(TARGET)
