# chapter_20/lab20-1 Makefile

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pthread
INCLUDES = -I./include

SRCS = src/main.cpp src/atomic_test.cpp
TARGET = atomic_demo

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)