File Transfer Facility
======================

File transfer client and server built using C, ZeroMQ and OpenGL/GLUT.

ftserver
--------

1. Type "make" in terminal to build ftserver.
2. Run 'ftserver.exe'.

ftclient
--------

1. Type 'make' in terminal to build ftclient_comms.
2. Type 'gcc -Wall -o ftclient_gui.exe ftclient_gui.c -lGL -lglut -lGLU' to build GUI.
3. Run 'ftclient.exe' or 'ftclient_gui.exe'

Note:
-----
-ftclient_comms is yet to be properly hooked into GUI. The GUI client is unfinished. 
-Code relies on older versions of ZeroMQ and OpenGL/GLUT (2011).
