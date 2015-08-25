/*******************************************************//**
 *  @file    ftserver_log.h
 *  @brief   ENEL323 Project #3: File Transfer Program  
 *  @brief   Server-side activity logging code.  
 * 
 *  @author  Aydin Arik
 *  @version 04/09/11         
 **********************************************************/

#ifndef FTSERVER_LOG_H
#define FTSERVER_LOG_H

/************************************************
 *              Global Constants
 ***********************************************/
enum possibleServerActivity {SERVER_START = 0, CLIENT_REQ, FILE_UPLOADED};


/************************************************
 *              Public Functions
 ***********************************************/
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
 *  @return 1 Failure to open a log file.     
 */
int logServerActivity (int serverActivity, char* portNum, char* dir, char* filename);

#endif

