CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = matrix_lab

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp

clean:
	rm -f $(TARGET)

.PHONY: all clean