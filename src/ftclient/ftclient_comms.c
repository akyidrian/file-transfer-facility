/*******************************************************//**
 *  @file    ftclient_comms.c
 *  @brief   ENEL323 Project #3: File Transfer Program  
 *  @brief   File transfer communication code for client.     
 * 
 *  @author  Aydin Arik
 *  @version 22/09/11         
 **********************************************************/
 
/************************************************
 *              Header Files
 ***********************************************/
#include <zmq.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "typedef.h"
#include "zhelpers.h"
#include "ftclient_comms.h"


/************************************************
 *              Private Constants
 ***********************************************/
enum negotiationResponses{INVALID_REPLY = -1, DO_NOT_TRANSFER_FILE, TRANSFER_FILE};
enum fileSaving{FILE_CAN_NOT_BE_SAVED = -1, FILE_IS_TOO_BIG, FILE_SAVED}; 
enum fileTransferStatus{FT_UNKNOWN = -1, FT_FAIL, FT_SUCCESS};
#define MAX_FILE_SIZE 2097152


/************************************************
 *              Functions
 ***********************************************/
int FTClientSideNegotiation (void *context, void *socket);
void terminateClientConnection (void *context, void *socket);
void *saveAFileIntoMem (char *fileName);
int sendFile (void *context, void *socket, file_t *file);
void storeStrChars (char *fileName, file_t *file);


/**
 *  Stores the filename of the file required to be uploaded to memory (file structure).
 *
 *  @param  filename Filename of file which is to be stored in memory. 
 *  @param  file The file structure for memory storage of file information. 
 */
void storeStrChars (char *fileName, file_t *file)
{
    char *stringPtr = fileName;
    int i = 0;
    
    while (*stringPtr != '\0')
    {
        file->fileName[i] = *stringPtr;
        
        stringPtr++;
        i++;
    }
}

/**
 *  Saves nominated file into memory.
 *
 *  @param  filename The name of the file to save.
 *
 *  @return file The file structure created of the file with name 'filename'.
 *  @return NULL An error occured whilst trying to save file.  
 */
void *saveAFileIntoMem (char *fileName)
{
    file_t *file = NULL;
    long fileSize = 0;
    
    FILE *fileToBeTransferred = fopen (fileName, "rb");
    
    if (fileToBeTransferred == NULL)
    {
        printf ("File can't be opened.\n");
        return (NULL); //FILE_CAN_NOT_BE_SAVED);
    }
    
    fseek (fileToBeTransferred, 0, SEEK_END);
    
    fileSize = ftell (fileToBeTransferred);
    
    file = malloc ( sizeof(file_t) + (fileSize * sizeof(unsigned char)) );
    
    if (file == NULL)
    {
        printf("Unable to allocate memory for filename and filedata storage.\n");
        return (NULL); //FILE_CAN_NOT_BE_SAVED);
    }
    
    storeStrChars (fileName, file);
    file->fileSize = fileSize;
    
    fseek(fileToBeTransferred, 0, SEEK_SET);
      
    fread (file->fileData, fileSize, 1, fileToBeTransferred);
    fclose (fileToBeTransferred); 
    
    return (file);
}

/**
 *  Client-server negotiation function for deciding whether client should sent
 *  file structure or not.
 *
 *  @param  context The context used for communications.
 *  @param  socket The socket the server communicates on with the client.  
 *
 *  @return TRANSFER_FILE 
 *  @return DO_NOT_TRANSFER_FILE   
 *  @return INVALID_REPLY File transfer situation is not known.    
 */
int FTClientSideNegotiation (void *context, void *socket)
{
    char *serverReply;
    
    s_send (socket, "FTREQ");

    serverReply = s_recv (socket);
    
    if((strncmp (serverReply, "TF", strlen ("TF"))) == 0)
    {
        printf ("Received %s.\n", serverReply);
        return (TRANSFER_FILE);
    }
    else if ((strncmp ((const char *)serverReply, "DNTF", strlen ("DNTF"))) == 0)
    {
        printf ("Received %s. Server is busy. Try again later.\n", serverReply);
        return (DO_NOT_TRANSFER_FILE);
    }
    else 
    {
        printf ("Invalid reply received. Try again later.\n");
        return (INVALID_REPLY);
    }
}

/**
 *  Sends file structure to server.
 *
 *  @param  context The context used for communications.
 *  @param  socket The socket the server communicates on with the client.  
 *  @param  file File information. Contains filename, filesize and filedata.
 *
 *  @return FT_SUCCESS File transfer was successful.
 *  @return FT_FAIL File transfer wasn't successful.  
 *  @return FT_UNKNOWN File transfer situation is not known.    
 */
int sendFile (void *context, void *socket, file_t *file)
{
    char *serverReply;
    
    zmq_msg_t request;
    zmq_msg_init_size (&request, sizeof(file_t) + (file->fileSize * sizeof(unsigned char)) );
    memcpy (zmq_msg_data (&request), file, sizeof(file_t) + (file->fileSize * sizeof(unsigned char)) );
    zmq_send (socket, &request, 0);
    zmq_msg_close (&request);

    serverReply = s_recv (socket); 

    if((strncmp ((const char *)serverReply, "FT_SUCCESS", strlen ("FT_SUCCESS"))) == 0)
    {
        printf ("Received %s.\n", serverReply);
        return (FT_SUCCESS);
    }
    else if ((strncmp (serverReply, "FT_FAIL", strlen ("FT_FAIL"))) == 0)
    {
        printf ("Received %s. Server is busy. Try again later.\n", serverReply);
        return (FT_FAIL);
    }
    else
    {
        printf ("Invalid or no reply received. Try again later.\n");
        return (FT_UNKNOWN);
    }
}

void terminateClientConnection (void *context, void *socket)
{
    zmq_close (socket);
    zmq_term (context);
}



/**
 *  Main entry point for client program.    
 */
int main (void)
{
    file_t *file = NULL;
    void *context = NULL;
    void *requester = NULL;
    int sendFlag = 0;
    int FTStatus = 0;
    char *fileName = "pic.jpg"; // Needs to be specified by glut.
    
    file = saveAFileIntoMem (fileName);
    

    
    
    if (file != NULL)
    {
            if (file->fileSize > MAX_FILE_SIZE)
            {
                fprintf (stderr, "File is too big.\n");
            }
        
            context = zmq_init (1);
    
            //  Socket to talk to server
            requester = zmq_socket (context, ZMQ_REQ);
            zmq_connect (requester, "tcp://localhost:5678"); 
            
            sendFlag = FTClientSideNegotiation (context, requester);// Needs to be specified by glut.
            
            switch (sendFlag)
            {
                case TRANSFER_FILE:
                    printf ("TRANSFER FILE.\n");
                    
                    FTStatus = sendFile (context, requester, file);
                     
                    switch (FTStatus)
                    {
                        case FT_SUCCESS:
                            printf("File was received by server.\n");
                            break;
                            
                        case FT_FAIL:
                            printf("File wasn't received by server.\n");
                            break;
                            
                        case FT_UNKNOWN:
                            printf("Invalid response was found during file transfer.\n");
                            break;
                            
                    }
                            
                    
                    break;
                    
                case DO_NOT_TRANSFER_FILE:
                    printf ("DO NOT TRANSFER FILE.\n");
                    break;
                    
                case INVALID_REPLY:
                    printf ("INVALID REPLY.\n");
                    break;
            }
    }
    else if (file == NULL)
    {
            printf("FILE CANNOT BE SAVED.\n");
    }
            
            
    terminateClientConnection (context, requester);
    free (file); // free allocated memory 
    
    return 0;
}
 
