CC = g++
CFLAGS = -Wall -Wextra -g

SRCS = main.cpp \
       Interface.cpp \
       Errors/Errors.cpp \

HEADERS = Interface.hpp \
		  IDictioanry/IDictioanry.hpp \
          Red-Black Tree/RedBlackTree.hpp \
          Errors/Errors.hpp \

OBJS = $(SRCS:.cpp=.o)

TARGET = lab2

.PHONY: all clean cleanall rebuild run rerun

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

cleanall:
	rm -f $(OBJS) $(TARGET)

rebuild: cleanall all

run:
	./$(TARGET)

rerun: rebuild run