

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -O2
LIBS = -lpthread
TARGET = server
SRCS = server.cpp doublyLinkedList.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run C++ server in the background
run-backend: $(TARGET)
	./$(TARGET) &

# Run Node.js frontend in the background
run-frontend:
	nohup node server.js > frontend.log 2>&1 &

# Run both servers
run-all: run-backend run-frontend
	@echo "Both servers are running."

# Stop both servers
stop:
	pkill -f server
	pkill -f node

clean:
	rm -f $(TARGET) $(OBJS)

