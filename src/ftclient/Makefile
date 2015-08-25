#
#  @file    Makefile
#  @brief   ENEL323 Project #3: File Transfer Program  
#  @brief   Makefile for client.   
# 
#  @author  Aydin Arik
#  @version 04/09/11         
#

# Definitions
TARGET = ftclient
CC = gcc
CFLAGS = -Wall
GLUTFLAG = -lglut
OPENGLFLAGS = -lGL -lGLU
ZEROMQFLAG = -lzmq
DEL = rm

# Default target
all: $(TARGET).exe

# Compile: Create object files from C source files
ftclient_comms.o: ftclient_comms.c ftclient_comms.h zhelpers.h typedef.h
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET).exe: ftclient_comms.o
	$(CC) $(CFLAGS) $^ -o $@ $(ZEROMQFLAG) 
    

# Remove old files
.PHONY: clean
clean:
	$(DEL) -f *.o *.exe 
