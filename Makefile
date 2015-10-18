# Select executable sufffix here
EXE_SFX=.app

# Select compiler here:
CXX=g++

# Select compilation flags here:
CFLAGS=-std=c++14 -c -Wall `pkg-config --cflags gtksourceview-3.0 gtkmm-3.0` -I../wren/src/include

# Select linker flags here:
LDFLAGS=`pkg-config --libs gtksourceview-3.0 gtkmm-3.0 libxml-2.0`

all: jazz.o jazz_init.o jazz_tablabel.o jazz_sourceview.o jazz_menucallback.o\
		 jazz_filetree.o jazz_wren.o
	$(CXX) $^ $(LDFLAGS) -L../wren/lib -lwren -o bin/jazz$(EXE_SFX)
	
jazz.o: jazz.cpp
	$(CXX) $(CFLAGS) jazz.cpp
	
jazz_init.o: jazz_init.cpp
	$(CXX) $(CFLAGS) jazz_init.cpp
	
jazz_tablabel.o: jazz_tablabel.cpp
	$(CXX) $(CFLAGS) jazz_tablabel.cpp
	
jazz_sourceview.o: jazz_sourceview.cpp
	$(CXX) $(CFLAGS) jazz_sourceview.cpp
	
jazz_menucallback.o: jazz_menucallback.cpp
	$(CXX) $(CFLAGS) jazz_menucallback.cpp
	
jazz_filetree.o: jazz_filetree.cpp
	$(CXX) $(CFLAGS) jazz_filetree.cpp
	
jazz_wren.o: jazz_wren.cpp
	$(CXX) $(CFLAGS) jazz_wren.cpp

clean:
	rm *.o
	
clean_all:
	rm *.o
	cd mruby && make clean
