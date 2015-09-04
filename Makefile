# Select executable sufffix here
EXE_SUFFIX=.app

# Select object file suffix here:
OBJ_SUFFIX=.o

# Select compiler here:
CPP=g++

# Select compilation flags here:
CFLAGS=-std=c++11 -c -Wall `pkg-config --cflags gtksourceview-3.0`

# Select linker flags here:
LDFLAGS=`pkg-config --libs gtksourceview-3.0 gtk+-3.0`

all: jazz.o
	$(CPP) jazz.o $(LDFLAGS) -o jazz$(EXE_SUFFIX)
	
jazz.o: jazz.cpp
	$(CPP) $(CFLAGS) jazz.cpp
	
clean:
	rm *$(OBJ_SUFFIX)
