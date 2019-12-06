CXX = g++ -std=c++11
CFLAGS = -g -Wall
DEPS = /usr/local/include/eigen3
CXXFILES = main.cpp

all: main.cpp
	$(CXX) -I $(DEPS) $(CXXFILES) -o main

.PHONY: clean
clean:
	rm output.ppm main
