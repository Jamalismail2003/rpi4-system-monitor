# QNX RPi4 System Monitor Makefile
# 
# Usage:
#   make              - Build for native platform
#   make clean        - Remove build artifacts
#   make install      - Install binary to /usr/bin
#
# Cross-compilation:
#   qcc -V gcc_ntoaarch64le rpi4-system-monitor.c -o rpi4-system-monitor

ifndef QNX_TARGET
QNX_TARGET=$(QNX_TARGET)
endif

PROGRAM = rpi4-system-monitor
SRCS = src/rpi4-system-monitor.c src/main.c
OBJS = $(SRCS:.c=.o)
HEADERS = include/rpi4-system-monitor.h

CFLAGS = -Wall -Wextra -std=c99 -I./include
LDFLAGS =

# Compiler selection
ifeq ($(TARGET),aarch64)
	CC = qcc -V gcc_ntoaarch64le
else
	CC = qcc
endif

.PHONY: all clean install

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(PROGRAM)

install: $(PROGRAM)
	@if [ -w /usr/bin ]; then \
		cp $(PROGRAM) /usr/bin/; \
		chmod 755 /usr/bin/$(PROGRAM); \
		echo "Installed $(PROGRAM) to /usr/bin"; \
	else \
		echo "Error: Cannot write to /usr/bin. Try 'sudo make install'"; \
		exit 1; \
	fi
