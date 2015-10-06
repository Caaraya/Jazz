# Select executable sufffix here
EXE_SFX=.app

# Select compiler here:
CXX=g++

# Select compilation flags here:
CFLAGS=-std=c++11 -c -Wall `pkg-config --cflags gtksourceview-3.0 gtkmm-3.0` -I./mruby/include

# Select linker flags here:
LDFLAGS=`pkg-config --libs gtksourceview-3.0 gtkmm-3.0 libxml-2.0` -L./mruby/build/host/lib -lmruby -lmruby_core

all: mruby.a jazz

mruby.a:
	cd mruby && make
	mkdir -p bin/mrbgems
	cp mruby/bin/* bin
	cp -avr mruby/build/host/mrbgems bin
	
	
jazz: jazz.o jazz_init.o jazz_tablabel.o jazz_sourceview.o jazz_menucallback.o\
		 jazz_filetree.o mrubybind.o
	$(CXX) $^ $(LDFLAGS) -o bin/jazz$(EXE_SFX)
	
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
	
mrubybind.o: mrubybind/mrubybind.cc
	$(CXX) -I./mruby/include -std=c++11 -c -Wall mrubybind/mrubybind.cc
	
geminit.o: bin/gem_init.c
	

clean:
	rm *.o
	
clean_all:
	rm *.o
	cd mruby && make clean
