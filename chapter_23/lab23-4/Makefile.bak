# Makefile for lab23-4: SVE strcmp

CXX = aarch64-linux-gnu-g++
CXXFLAGS = -std=c++17 -Wall -O2 -march=armv8-a+sve
INCLUDES = -I./include

TARGET = sve_strcmp_test
SRCS = src/strcmp_sve.S src/main.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)