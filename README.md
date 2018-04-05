File Transfer Facility
======================

File transfer client and server built using ZeroMQ and OpenGL/GLUT.

ftserver
--------

1. 'cd' to 'src/ftserver'
2. Type 'make' to build ftserver
3. Run 'ftserver.exe' using './ftserver.exe'

ftclient
--------

1. 'cd' to 'src/ftclient'
2. Type 'make' in terminal to build ftclient
3. Type 'gcc -Wall -o ftclient_gui.exe ftclient_gui.c -lGL -lglut -lGLU' to build ftclient_gui
4. Run 'ftclient.exe' using './ftclient.exe [file_to_transfer_path]'
5. To see GUI, run 'ftclient_gui.exe' using './ftclient_gui.exe'

Note
----
* ftclient_comms is not hooked in with ftclient_gui. The client GUI is unfinished.
* Code relies on older versions of ZeroMQ and OpenGL/GLUT (2011 or earlier).
