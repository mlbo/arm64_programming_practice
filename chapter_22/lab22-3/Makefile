# Makefile for lab22-3: Auto-vectorization vs Manual NEON

CXX = aarch64-linux-gnu-g++
CXXFLAGS = -std=c++17 -Wall -O3 -march=armv8-a+simd -ftree-vectorize
INCLUDES = -I./include

TARGET = auto_vectorization_test
SRCS = src/auto_vec.cpp src/main.cpp

.PHONY: all clean run vec-report

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

# Show vectorization report
vec-report:
	$(CXX) -O3 -march=armv8-a+simd -fopt-info-vec $(INCLUDES) -c $(SRCS)