/*******************************************************//**
 *  @file    ftserver_log.c
 *  @brief   ENEL323 Project #3: File Transfer Program  
 *  @brief   Server-side activity logging code.  
 * 
 *  @author  Aydin Arik
 *  @version 04/09/11        
 **********************************************************/

/************************************************
 *              Header Files
 ***********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ftserver_log.h"

/************************************************
 *              Public Functions
 ***********************************************/
int logServerActivity (int serverActivity, char* portNum, char* dir, char* filename);


/**
 *  Records server-side activity into a log file called "ftserver.log". Server 
 *  activities may include SERVER_START, CLIENT_REQ and FILE_UPLOADED.
 *
 *  @param  serverActivity Server activity to be recorded in log file.
 *  @param  portNum Port-number listened in on by server.
 *  @param  dir Directory uploaded files are saved in by server.
 *  @param  filename Filenames of uploaded files to server.  
 *
 *  @return 0 Activity successfully logged.
 *  @return 1 Log file could not be opened. No activity is recorded.     
 */
int logServerActivity (int serverActivity, char* portNum, char* dir, char* filename)
{
    time_t currTime;
    time(&currTime);   
    FILE *logFile = fopen("ftserver.log", "ab");
    
    if (logFile == NULL)
    {
        fprintf(stderr, "Log file can't be opened.\n");
        return(1);
    }
    
    
    if (serverActivity == SERVER_START)
    {
        fprintf (logFile, "==============================================================\n");
        fprintf (logFile, "                    %s", ctime(&currTime));
        fprintf (logFile, "==============================================================\n");
        fprintf (logFile, "   ftserver booted.\n   Listening to port number %s.\n   Files were saved in %s\n", portNum, dir);
        fprintf (logFile, "===============================================================\n\n");
    }    
    else if (serverActivity == CLIENT_REQ)
    {
        fprintf (logFile, "===============================================================\n");
        fprintf (logFile, "                    %s", ctime(&currTime));
        fprintf (logFile, "===============================================================\n");
        fprintf (logFile, "   File upload request received.\n");
        fprintf (logFile, "===============================================================\n\n");
    }
    else if (serverActivity == FILE_UPLOADED)
    {
        fprintf (logFile, "===============================================================\n");
        fprintf (logFile, "                    %s", ctime(&currTime));
        fprintf (logFile, "===============================================================\n");
        fprintf (logFile, "   File uploaded: \n   %s\n",filename);
        fprintf (logFile, "===============================================================\n\n");
    }
    
    fclose(logFile);
    
    
    return (0);   
}
