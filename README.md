# Jazz
A code editor written in C++ with Gtk

## Dependancies
- gtkmm3
- gtksourceview >= 3.14

## Windows
using msys2 in the win64 or 32 shell, cmd: pacman -S mingw-w64-x86_64-gtkmm3 mingw-w64-x86_64-gtksourceview3

### Working features
- Auto suggestions / completion
- Saving, opening and creating files
- Debugger has a few features:
  - Starting gdb instance
  - Setting breakpoints in project file
  - Continue execution
- File tree works but path is hardcoded to cwd

## To compile
- git submodule init
- git submodule update
- cd coralscript && make
- cd ../ && make
