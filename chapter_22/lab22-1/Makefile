# Makefile for lab22-1: NEON RGB/BGR Conversion

CXX = aarch64-linux-gnu-g++
CXXFLAGS = -std=c++17 -Wall -O2 -march=armv8-a+simd
INCLUDES = -I./include

TARGET = neon_rgb_bgr_test
SRCS = src/neon_rgb_bgr.cpp src/main.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)