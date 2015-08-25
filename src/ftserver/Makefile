#
#  @file    Makefile
#  @brief   ENEL323 Project #3: File Transfer Program  
#  @brief   Makefile for server.   
# 
#  @author  Aydin Arik
#  @version 04/09/11         
#

# Definitions
TARGET = ftserver
CC = gcc
CFLAGS = -Wall
ZEROMQFLAG = -lzmq
DEL = rm

# Default target
all: $(TARGET).exe

# Compile: Create object files from C source files
ftserver_comms.o: ftserver_comms.c ftserver_comms.h zhelpers.h typedef.h ftserver_log.h
	$(CC) -c $(CFLAGS) $< -o $@

ftserver_main.o: ftserver_main.c ftserver_comms.h ftserver_log.h
	$(CC) -c $(CFLAGS) $< -o $@

ftserver_log.o: ftserver_log.c ftserver_log.h
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET).exe: ftserver_comms.o ftserver_main.o ftserver_log.o
	$(CC) $(CFLAGS) $^ -o $@ $(ZEROMQFLAG) 
    

# Remove old files
.PHONY: clean
clean:
	$(DEL) -f *.o *.exe *.log 
