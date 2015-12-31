# Select compiler here:
CXX=g++

# Select compilation flags here:
CFLAGS=-std=c++14 -Os -c -Wall `pkg-config --cflags gtksourceview-3.0 gtkmm-3.0`

# Select linker flags here:
LDFLAGS=`pkg-config --libs gtksourceview-3.0 gtkmm-3.0 libxml-2.0` -L./coralscript/bin -lcoral

all: jazz.o jazz_init.o jazz_tablabel.o jazz_sourceview.o jazz_menucallback.o\
		 jazz_filetree.o jazz_projecttree.o jazz_newproj_dialog.o jazz_msgbox.o
	$(CXX) $^ $(LDFLAGS) -o bin/jazz
	
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

jazz_projecttree.o: jazz_projecttree.cpp
	$(CXX) $(CFLAGS) jazz_projecttree.cpp
	
jazz_newproj_dialog.o: jazz_newproj_dialog.cpp
	$(CXX) $(CFLAGS) jazz_newproj_dialog.cpp

jazz_msgbox.o: jazz_msgbox.cpp
	$(CXX) $(CFLAGS) jazz_msgbox.cpp

clean:
	rm -f *.o
