# Makefile for lab23-5: SVE Matrix Multiplication

CXX = aarch64-linux-gnu-g++
CXXFLAGS = -std=c++17 -Wall -O2 -march=armv8-a+sve
INCLUDES = -I./include

TARGET = sve_matrix_test
SRCS = src/sve_matrix.cpp src/main.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)