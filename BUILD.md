This is the README file for colibri-0.1.1, released 10-June-2002.

Version 0.1.1 supports two boards:
* lego_rcx
* hms_evb_h8s2633f

Generic build instructions:

1. Make sure that you have a h8-configured binutils/gcc handy.
   I use h8300-hms as target with binutils-2.11.2 and gcc-3.0.3.
2. $ make config BOARD=your-board
3. $ make CROSS=/path_to_your_compiler/prefix-
4. use colibri.srec together with colibri.map

Read the board-specific README for more information.

Feel free to contact me at <damm@opensource.se>

Have fun!

/ Magnus 
