/*******************************************************//**
 *  @file    typedef.h
 *  @brief   ENEL323 Project #3: File Transfer Program  
 *  @brief   Contains file typedef used by server & client.   
 * 
 *  @author  Aydin Arik
 *  @version 04/09/11         
 **********************************************************/
 
#ifndef TYPEDEF_H
#define TYPEDEF_H 
/************************************************
 *             Global Constants
 ***********************************************/
#define MAX_FILENAME_SIZE 256   // Note: 256 byte filenames are the max. on LINUX. 


/************************************************
 *             Global Type Definitions
 ***********************************************/
typedef struct
{
    char fileName[MAX_FILENAME_SIZE];   
    long fileSize;
    unsigned char fileData[]; // Variable length array (C99 standard). Number of array elements decided at run-time rather than compile time.
} file_t; 

#endif
