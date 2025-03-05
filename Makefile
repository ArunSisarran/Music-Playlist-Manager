
CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lncurses -lcurl

SRC = src/test.cpp src/Display/display.cpp src/LinkedList/doublyLinkedList.cpp src/LinksFunction/link.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

