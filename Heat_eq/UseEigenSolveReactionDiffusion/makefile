CXX = g++
TARGET = a
CXXFLAGS = -std=c++14 -Wall -Wextra -O3 -I/opt/local/include -I/usr/local/include
SRCS = main.cpp
OBJS :=$(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

#run: all
#	./$(TARGET)

all: clean $(TARGET)

clean:
	rm $(TARGET) $(OBJS)

