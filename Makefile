# Select executable sufffix here
EXE_SFX=.app

# Select compiler here:
CPP=g++

# Select compilation flags here:
CFLAGS=-std=c++11 -c -Wall `pkg-config --cflags gtksourceview-3.0`

# Select linker flags here:
LDFLAGS=`pkg-config --libs gtksourceview-3.0 gtk+-3.0 libxml-2.0`

all: jazz.o
	$(CPP) $^ $(LDFLAGS) -o jazz$(EXE_SFX)
	
jazz.o: jazz.cpp
	$(CPP) $(CFLAGS) jazz.cpp
	
clean:
	rm *.o
