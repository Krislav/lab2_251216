CC = g++
CFLAGS = -Wall -Wextra -g
LDFLAGS = -mwindows -lcomctl32

SRCS = WinMain.cpp \
       Errors/Errors.cpp \
	   Tests/Tests.cpp \
	   Tests/TestShell.cpp \

HEADERS = IDictionary/IDictionary.hpp \
          Red-Black Tree/RedBlackTree.hpp \
          Errors/Errors.hpp \
		  Tests/Tests.hpp \
		  Tests/TestShell.hpp \
		  Book/Book.hpp

OBJS = $(SRCS:.cpp=.o)

TARGET = lab2

.PHONY: all clean cleanall rebuild run rerun

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

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