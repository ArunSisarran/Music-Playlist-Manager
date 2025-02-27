

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -O2
LIBS = -lpthread
TARGET = server
SRCS = test. cpp doublyLinkedList.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
# Run C++ server in the background
run: $(TARGET)
	./$(TARGET) &
clean:
	rm -f $(TARGET) $(OBJS)

