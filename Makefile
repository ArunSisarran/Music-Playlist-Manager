CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
LDFLAGS = -lncurses -lcurl -pthread
SRC = src/test.cpp \
      src/Display/display.cpp \
      src/LinkedList/doublyLinkedList.cpp \
      src/LinksFunction/link.cpp \
      src/SpotifyPlayer/spotifyPlayer.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = program

# Include directories
INCLUDES = -Isrc/Display \
           -Isrc/LinkedList \
           -Isrc/LinksFunction \
           -Isrc/SpotifyPlayer

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: all
	./$(TARGET)

debug: CXXFLAGS += -DDEBUG
debug: all

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all run clean debug
