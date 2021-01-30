# System-Monitor
System Monitor for linux, build using the ncurses text-based graphical library, displays cpu and memory utitization for the system as well as the processes. The information of system resources in Linux is stored in /proc folder and is taken and processed as such.


## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

## Instructions

1. <code>mkdir build && cd build</code>
2. <code>cmake .. && make</code>
3. <code>./monitor</code>