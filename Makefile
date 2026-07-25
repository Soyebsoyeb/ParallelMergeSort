CXX = g++
CXXFLAGS = -std=c++11 -O2 -pthread -Wall
TARGET = parallel_sort
SOURCES = src/app/main.cpp \
          src/app/sorting/mergeSort.cpp \
          src/app/sorting/parallelMergeSort.cpp \
          src/app/sorting/parallelMergeSortThreadPool.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
