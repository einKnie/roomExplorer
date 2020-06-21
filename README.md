# Room Explorer

A boring terminal-based dungeon crawler.  
This project is still a work in progress. I see it as more of a coding exercise instead of an actual game.

## Compilation

### Prerequisites
This program uses the logging functionality provided by [Logger](https://github.com/einKnie/Logger).  
Clone the [Logger](https://github.com/einKnie/Logger) repo to your machine and compile the library. Then change the variable LIB_DIR of the RoomExplorer's Makefile to point to the libcpplogging library.

### Compilation
To compile, cd into the repo and call <i>make -C ./proj</i>
The resulting binary can be found in /bin.

### Repo Structure
log<br>
&nbsp;&nbsp;|_ bin/   &nbsp;&nbsp;&nbsp;&nbsp;...binary files<br>
&nbsp;&nbsp;|_ doc/   &nbsp;&nbsp;&nbsp;...documentation<br>
&nbsp;&nbsp;|_ inc/   &nbsp;&nbsp;&nbsp;&nbsp;...header files<br>
&nbsp;&nbsp;|_ proj/  &nbsp;&nbsp;&nbsp;...build files<br>
&nbsp;&nbsp;|_ src/   &nbsp;&nbsp;&nbsp;&nbsp;...source files<br>
