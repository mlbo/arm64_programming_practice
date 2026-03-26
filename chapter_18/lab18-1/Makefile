# chapter_18/lab18-1 Makefile

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pthread
INCLUDES = -I./include

SRCS = src/main.cpp src/litmus_tests.cpp
TARGET = litmus_test

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)