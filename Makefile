# Select executable sufffix here
EXE_SFX=.app

# Select compiler here:
CXX=g++

# Select compilation flags here:
CFLAGS=-std=c++11 -c -Wall `pkg-config --cflags gtksourceview-3.0 gtkmm-3.0`

# Select linker flags here:
LDFLAGS=`pkg-config --libs gtksourceview-3.0 gtkmm-3.0 libxml-2.0`

all: jazz.o jazz_init.o
	$(CXX) $^ $(LDFLAGS) -o jazz$(EXE_SFX)
	
jazz.o: jazz.cpp
	$(CXX) $(CFLAGS) jazz.cpp
	
jazz_init.o: jazz_init.cpp
	$(CXX) $(CFLAGS) jazz_init.cpp
	
clean:
	rm *.o
