# Jazz
A code editor written in C++ with Gtk

## Dependancies
- gtkmm3
- gtksourceview >= 3.14

At the moment jazz is using wren experimentally for scripting support.
Clone it in the samee directory that the Jazz dir resides in and run: make vm inside of wren

## Windows
using msys2, cmd: pacman -S mingw-w64-x86_64-gtkmm3 mingw-w64-x86_64-gtksourceview3

### Working features
- Auto suggestions / completion
- Saving, opening and creating files

### Todos
- File tree works but only shows cwd

## To compile
Linux, run: make
Windows, run: make EXE_SFX=.exe