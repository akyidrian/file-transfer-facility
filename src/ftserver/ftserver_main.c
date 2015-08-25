/*******************************************************//**
 *  @file    ftserver_main.c
 *  @brief   ENEL323 Project #3: File Transfer Program  
 *  @brief   Main entry point for server program.   
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
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include "ftserver_comms.h"
#include "ftserver_log.h"

/************************************************
 *              Private Constants
 ***********************************************/
#define TCP_IP_STRLEN 14
#define HELP_OPT_CHAR 'h'    
#define DIR_OPT_CHAR 'd' 
#define PORT_OPT_CHAR 'p' 
#define DEFAULT_PORT_NUM "5678"
#define MAX_NUM_OF_PORT_DIGITS 4
#define DIR_STRLEN 1024

/************************************************
 *              Private Functions
 ***********************************************/
static int checkIfValidPortNum (char* portNumberStr);

/************************************************
 *              File Global Variables
 ***********************************************/
static const char *helpMessage = 
"\n ***************************************************************\n"
" *              Welcome to the ftserver Help Menu.             *\n"
" ***************************************************************\n"
" *   Options available:                                        *\n"
" *                                                             *\n"
" *   -d <directory-path>                                       *\n"
" *      Specify the directory to save uploaded files and a     *\n"
" *      log file.The log file records all ftserver activity.   *\n"        
" *                                                             *\n"
" *      DEFAULT directory is ftserver's current working        *\n"
" *      directory.                                             *\n"
" *                                                             *\n"
" *                                                             *\n"
" *   -p <port-number>                                          *\n"
" *      Specify port number ftserver will listen in on.        *\n"
" *      Port range is 1-9999.                                  *\n"
" *                                                             *\n"
" *      DEFAULT port setting is 5678.                          *\n"
" *                                                             *\n"
" ***************************************************************\n"
" *           To terminate ftserver press \"CTRL+C\"              *\n"
" ***************************************************************\n\n";



/**
 *  Checks if valid TCP/IP port number is entered using command line options.
 */
static int checkIfValidPortNum (char* portNumberStr)
{
    int portNumber = 0;
    int tempDigit = 0;
    int i = 0;
    
    for (i = 0; portNumberStr[i] != '\0'; i++)
    {
         switch (portNumberStr[i])
         {
             case '0':
                 tempDigit = 0;
                 break;
             case '1':
                 tempDigit = 1;
                 break;
             case '2':
                 tempDigit = 2;
                 break;
             case '3':
                 tempDigit = 3;
                 break;
             case '4':
                 tempDigit = 4;             
                 break;
             case '5':
                 tempDigit = 5;
                 break;
             case '6':
                 tempDigit = 6;
                 break;
             case '7':
                 tempDigit = 7;
                 break;
             case '8':
                 tempDigit = 8;
                 break;
             case '9':
                 tempDigit = 9;
                 break;
             default:
                 fprintf (stderr, "NaN\n");
                 return (-1);
         }
         
         if (i == 0)
         {
             portNumber = tempDigit;
         }
         else
         {
            portNumber = 10 * portNumber + tempDigit;   
         }
     }  
     
     if (portNumber < 1)
     {
         printf ("Port number is below 1.\n");
         return (-2);
     }
     else if (portNumber > 9999)
     {
         printf ("Port number is greater than 9999.\n");
         return (-3);
     }
     else 
     {
         printf ("Port number of %d was chosen.\n", portNumber);
         return (portNumber);
     }
    
}


/**
 *  Main entry point for server program.
 */
int main (int argc, char **argv)
{    
    void *context = NULL;
    void *responder = NULL;
    char TCPIPStr[TCP_IP_STRLEN];
    char dir[DIR_STRLEN];
    char *portNumberStr = DEFAULT_PORT_NUM;
    int index; // Required to print out any non-option arguements.
    int optChar;
    opterr = 0; // Stops getopt() printing error messages.
    int validPortNumFlag = 0; // For ensuring port number is valid.
    DIR *dirPath = NULL; //For directory path checking.
    char cwd[DIR_STRLEN]; // ftserver's current working directory.
    
    /* Determining ftserver's current working directory (DEFAULT file saving directory) */
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        strcpy(dir, cwd);
    }
    else
        perror("getcwd() error");
    
    /* Checking command line options chosen; used to help setup ftserver */
    while ((optChar = getopt (argc, argv, "hd:p:")) != -1)
    {    
        switch (optChar)
        {
        case HELP_OPT_CHAR:
            fprintf (stdout, "%s", helpMessage);
            break;
        case DIR_OPT_CHAR:         
            dirPath = opendir(optarg);
            if (dirPath == NULL) // Test to see if directory exists.
            {
                perror("opendir"); // Need to choose a different directory or make the directory not found.
                exit (EXIT_FAILURE);
            }
            strcpy(dir, optarg);
            closedir(dirPath);
            
            break;
        case PORT_OPT_CHAR:
            validPortNumFlag = checkIfValidPortNum (optarg);
            
            if (validPortNumFlag < 0)//Lowest possible port
            {
                fprintf (stdout, "%s", helpMessage);
                exit (EXIT_FAILURE);
            }
            portNumberStr = optarg;
            
            break;
        case '?': // Found an invalid option or an option mssing an arguement.
            if ((optopt == DIR_OPT_CHAR) || (optopt == PORT_OPT_CHAR))
            {
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);//FIXME
            }   
            else if (isprint (optopt)) // Printable char?
            {
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);//FIXME
                fprintf (stdout, "%s", helpMessage);
            }
            else
            {
                fprintf (stderr, "Unknown option character `0x%x'.\n", optopt);//FIXME
                fprintf (stdout, "%s", helpMessage);
            }
            
            return (1);//FIXME:dont need?
        default:
            abort ();
        }
    }
       
    for (index = optind; index < argc; index++)
    {
        printf ("Non-option argument %s\n", argv[index]);//FIXME
        
        //fprintf (stdout, "%s", helpMessage);//FIXME????
    }
    
    /* Setup TCP/IP address and port number string for zmq_bind() */
    strcpy(TCPIPStr, "tcp://*:");
    strcat(TCPIPStr, portNumberStr); 

    context = zmq_init (1);

    //  Socket to talk to clients
    responder = zmq_socket (context, ZMQ_REP);
    
    // Configuring TCP/IP address and portnumber
    zmq_bind (responder, TCPIPStr);//TODO
    
    logServerActivity (SERVER_START, portNumberStr, dir, " "); // Record ftserver start in log.
    runServer (responder, dir);
    
    terminateServerConnection(context, responder);
    
    return (0);
}
