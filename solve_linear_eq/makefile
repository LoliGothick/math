CXX = g++
TARGET = a
CXXFLAGS = -std=c++14 -Wall -Wextra -O3
SRCS = main.cpp
OBJS :=$(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

#run: all
#	./$(TARGET)

all: clean $(TARGET)

clean:
	rm $(TARGET) $(OBJS)



#a: main.o point2D.o
#	g++ -std=c++14 -Wall -Wextra -o a main.o point2D.o
#main.o: main.cpp
#	g++ -std=c++14 -Wall -Wextra -c main.cpp
#point2D.o: point2D.cpp
#	g++ -std=c++14 -Wall -Wextra -c point2D.cpp

#clean:
#	rm *.o a
