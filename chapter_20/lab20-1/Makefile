# chapter_20/lab20-1 Makefile

ARMGNU ?= aarch64-linux-gnu

CXX = $(ARMGNU)-g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pthread
INCLUDES = -I./include

SRCS = src/main.cpp src/atomic_test.cpp
TARGET = atomic_demo

QEMU_USER = qemu-aarch64
QEMU_LIB = /usr/aarch64-linux-gnu

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -static -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	$(QEMU_USER) -L $(QEMU_LIB) -cpu max ./$(TARGET)