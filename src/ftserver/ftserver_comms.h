/*******************************************************//**
 *  @file    ftserver_comms.h
 *  @brief   ENEL323 Project #3: File Transfer Program  
 *  @brief   File transfer communications code for server.   
 * 
 *  @author  Aydin Arik
 *  @version 04/09/11         
 **********************************************************/
 
#ifndef FTSERVER_COMMS_H
#define FTSERVER_COMMS_H
/************************************************
 *              Public Functions
 ***********************************************/

/**
 *  Runs the servers communications.
 *
 *  @param  socket The socket the server communicates on with the client.  
 *  @param  saveDir Directory which files are saved.
 */
void runServer (void *socket, char* saveDir);

/**
 *  Stops server communications.
 *
 *  @param  context The context used for communications.
 *  @param  socket The socket the server communicates on with the client.   
 */
void terminateServerConnection (void *context, void *socket);


#endif
