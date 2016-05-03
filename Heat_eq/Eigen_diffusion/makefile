CXX = g++
TARGET = a
CXXFLAGS = -std=c++14 -Wall -Wextra -O3 -I/opt/local/include -I/usr/local/include
SRCS = main.cpp
OBJS :=$(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

RUN=./a

#run: all
#	./$(TARGET)

all: clean $(TARGET)

clean:
	rm $(TARGET) $(OBJS)

<<<<<<< HEAD
run:all
	${RUN}


#a: main.o point2D.o
#	g++ -std=c++14 -Wall -Wextra -o a main.o point2D.o
#main.o: main.cpp
#	g++ -std=c++14 -Wall -Wextra -c main.cpp
#point2D.o: point2D.cpp
#	g++ -std=c++14 -Wall -Wextra -c point2D.cpp

#clean:
#	rm *.o a
=======
>>>>>>> b2afad53f29b0bb3d92f65cf7158bd8074e05c57
