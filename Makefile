CXX=g++
CFLAGS=-std=c++11 -O2 -pthread
SRC=src/app/main.cpp src/app/sorting/mergeSort.cpp src/app/sorting/parallelMergeSort.cpp
TARGET=parallel_sort

all: $(TARGET)
$(TARGET): $(SRC)
	$(CXX) $(CFLAGS) $(SRC) -o $(TARGET)
clean:
	rm -f $(TARGET)
run: $(TARGET)
	./$(TARGET)
.PHONY: all clean run