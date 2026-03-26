# Makefile for lab23-2: SVE Interleaved Load/Store

CXX = aarch64-linux-gnu-g++
CXXFLAGS = -std=c++17 -Wall -O2 -march=armv8-a+sve
INCLUDES = -I./include

TARGET = sve_interleaved_test
SRCS = src/sve_interleaved.cpp src/main.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)