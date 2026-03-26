# Makefile for lab22-2: NEON Matrix Multiplication

CXX = aarch64-linux-gnu-g++
CXXFLAGS = -std=c++17 -Wall -O2 -march=armv8-a+simd
INCLUDES = -I./include

TARGET = neon_matrix_test
SRCS = src/neon_matrix.cpp src/main.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)