# chapter_16/lab16-1 Makefile

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pthread
INCLUDES = -I./include

SRCS = src/main.cpp src/false_sharing.cpp
TARGET = false_sharing_demo

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)