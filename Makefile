CXX = g++ -std=c++11
CFLAGS = -g -Wall
DEPS = /usr/local/include/eigen3
SRC_DIR = src
CXXFILES = $(SRC_DIR)/main.cpp

all: $(SRC_DIR)/main.cpp
	$(CXX) -Iinclude -I $(DEPS) $(CXXFILES) -o main

.PHONY: clean
clean:
	rm output.ppm main
