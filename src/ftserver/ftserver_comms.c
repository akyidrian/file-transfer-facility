/*******************************************************//**
 *  @file    ftserver_comms.c
 *  @brief   ENEL323 Project #3: File Transfer Program  
 *  @brief   File transfer communications code for server.   
 * 
 *  @author  Aydin Arik
 *  @version 04/09/11         
 **********************************************************/
 
/************************************************
 *              Header Files
 ***********************************************/
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "typedef.h"
#include "zhelpers.h"
#include "ftserver_comms.h"
#include "ftserver_log.h"

/************************************************
 *              Public Constants
 ***********************************************/
#define TRANSFER_FILE "TF"
#define FILE_TRANSFER_REQUEST "FTREQ"
#define FILE_TRANSFER_SUCCESS "FT_SUCCESS"

/************************************************
 *              Public Functions
 ***********************************************/
void runServer (void *socket, char* saveDir);
void terminateServerConnection (void *context, void *socket);


/************************************************
 *              Private Functions
 ***********************************************/
static int saveFile (file_t* file, char* saveDir);


/**
 *  Runs the servers communications.
 *
 *  @param  socket The socket the server communicates on with the client.  
 *  @param  saveDir Directory which files are saved.
 */
void runServer (void *socket, char* saveDir)
{
    file_t *file = NULL;
    while (1) 
    {
        void *clientRequest = NULL; // This may be used for a string-like message (has no NULL char) or file structure message (file_t). 

        //  Wait for next request from client
        zmq_msg_t request;
        zmq_msg_init (&request);
        zmq_recv (socket, &request, 0);
        clientRequest = zmq_msg_data (&request);        
        zmq_msg_close (&request);
        
        //  Do some 'work'
        sleep (1);
   
        if(strncmp ((const char *)clientRequest, FILE_TRANSFER_REQUEST, strlen(FILE_TRANSFER_REQUEST)) == 0)
        {     
            logServerActivity (CLIENT_REQ, " ", " ", " "); // Record client request into log.
            //  Send reply back to client
            s_send (socket, TRANSFER_FILE); 
            
            zmq_msg_init (&request);
            zmq_recv (socket, &request, 0);
            file = zmq_msg_data (&request);        
        
            //  Do some 'work'
            sleep (1);
            
            saveFile (file, saveDir);//TODO
            s_send (socket, FILE_TRANSFER_SUCCESS); 
            
            zmq_msg_close (&request); // Must let the file be saved first before doing this...
        }
        else
        {
            printf("DO_NOT_SEND_FILE\n");//FIXME
        }
    } 
}

/**
 *  Saves files in memory to a directory on a hard-drive.
 *
 *  @param  file File information. Contains filename, filesize and filedata.
 *  @param  saveDir Directory which files are saved.
 */
static int saveFile (file_t* file, char* saveDir)
{
    char saveFile[1024];
    
    strcpy(saveFile, saveDir);
    
    if (saveFile[strlen(saveFile) - 1] != '/')
        strcat(saveFile, "/");
    
    strcat(saveFile, &(file->fileName[0]));
    
    printf("File saved in: %s\n", saveFile);
    
    FILE *fileReceived = fopen(saveFile, "wb");
    
    if (fileReceived == NULL)
    {
        fprintf(stderr, "The file received could not be saved.\n");
        return (1);
    }
    logServerActivity (FILE_UPLOADED, " ", " ", file->fileName); // Record file upload information in log.
    
    fwrite(file->fileData, file->fileSize, 1, fileReceived);
    fclose(fileReceived);
    
    return (0);
}


/**
 *  Stops server communications.
 *
 *  @param  context The context used for communications.
 *  @param  socket The socket the server communicates on with the client.   
 */
void terminateServerConnection (void *context, void *socket)
{
    zmq_close (socket);
    zmq_term (context);
}



