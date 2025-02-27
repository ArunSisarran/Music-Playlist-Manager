
# Define the compiler and compiler flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Define the source files
SRCS = doublyLinkedList.cpp test.cpp display.cpp

# Define the object files
OBJS = $(SRCS:.cpp=.o)

# Define the output executable
TARGET = program

# Rule for building the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) -lncurses

# Rule for building object files from source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony target for clean
.PHONY: clean

