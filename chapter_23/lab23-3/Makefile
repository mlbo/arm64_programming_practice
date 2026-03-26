# Makefile for lab23-3: SVE memcpy

CXX = aarch64-linux-gnu-g++
CXXFLAGS = -std=c++17 -Wall -O2 -march=armv8-a+sve
INCLUDES = -I./include

TARGET = sve_memcpy_test
SRCS = src/sve_memcpy.cpp src/sve_memcpy.S src/main.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)