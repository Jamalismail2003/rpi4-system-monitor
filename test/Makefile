# Compiler for QNX
CXX = qcc
CC = qcc

# Target Architecture for RPi4 (64-bit ARM)
PLATFORM = -Vgcc_ntoaarch64le

# Compiler Flags
# For C code: standard C compilation
CFLAGS = $(PLATFORM) -Wall -I../include

# For C++ test code: C++ compilation with gTest
CXXFLAGS = $(PLATFORM) -lang-c++ -Wall -I../include

# GoogleTest Libraries provided by QNX
# -lgtest_main provides the main() entry point for the test
# -lgtest is the core framework
# -lregex is required by libgtest on QNX
# -lc++ for libc++
LIBS = -lgtest -lgtest_main -lregex -lc++

# Output binary name
TARGET = run_rpi4_system_monitor_tests

# Source files
C_SRCS = ../src/rpi4-system-monitor.c
CPP_SRCS = rpi4_system_monitor_test.cpp
C_OBJS = rpi4-system-monitor.o
CPP_OBJS = rpi4_system_monitor_test.o
OBJS = $(C_OBJS) $(CPP_OBJS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Compile C source file
$(C_OBJS): $(C_SRCS)
	$(CC) $(CFLAGS) -c $(C_SRCS) -o $@

# Compile C++ test file
$(CPP_OBJS): $(CPP_SRCS)
	$(CXX) $(CXXFLAGS) -c $(CPP_SRCS) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean run
