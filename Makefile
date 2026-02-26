CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRC = src/main.cpp src/scheduler.cpp
OUT = scheduler

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)