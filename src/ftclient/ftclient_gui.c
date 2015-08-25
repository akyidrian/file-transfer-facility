/* Header Files */
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>

/********************************************************************************************************
*                        Type Definitions
********************************************************************************************************/
typedef struct
{
   char *buttonString;
   int buttonPositionX;
   int buttonPositionY;
   int buttonState;
   int buttonType;
   int buttonHeight;
   int buttonWidth;
   int buttonFunc;
   float textColour[3];   // {R,G,B}
   float buttonColour[3]; // {R,G,B}
   float borderColour[3]; // {R,G,B}
} button_t;


/********************************************************************************************************
*                        Global Constants
********************************************************************************************************/
enum windowDimensions{DEFAULT_WIN_WIDTH = 400, DEFAULT_WIN_HEIGHT = 500}; /* Window is always this size */
enum popUpWindowDimensions{POP_UP_WIN_WIDTH = 200, POP_UP_WIN_HEIGHT = 100};
enum windowStartPos{DEFAULT_WIN_POSITION_X = 100, DEFAULT_WIN_POSITION_Y = 100}; 

/* BUT_CENTRE_TEXT_Y is required to centre the text (GLUT_BITMAP_HELVETICA_18) in the button */
enum specialButtonDimensions{BUT_GAP = 10, BUT_CENTRE_TEXT_Y = 6}; 
enum buttonStates{BUT_OUT = 0, BUT_OVER, BUT_PRESSED, BUT_IN}; 



enum normButtonDimensions{NORM_BUT_TYPE = 0, NORM_BUT_BORDER = 4, NORM_BUT_HEIGHT = 40, NORM_BUT_WIDTH = 100};

enum uploadButton{UPLOAD_BUT_CENTRE_X = DEFAULT_WIN_WIDTH - ((3 * NORM_BUT_WIDTH) / 2) - 2 * BUT_GAP, 
                  UPLOAD_BUT_CENTRE_Y = DEFAULT_WIN_HEIGHT - (NORM_BUT_HEIGHT / 2) - BUT_GAP, UPLOAD_BUT = 0};
enum exitButton{EXIT_BUT_CENTRE_X = DEFAULT_WIN_WIDTH - (NORM_BUT_WIDTH / 2) - BUT_GAP, 
                EXIT_BUT_CENTRE_Y = DEFAULT_WIN_HEIGHT - (NORM_BUT_HEIGHT / 2) - BUT_GAP, EXIT_BUT = 1};
enum popUpExitYesButton{POP_UP_EXIT_YES_BUT_CENTRE_X = DEFAULT_WIN_WIDTH - ((3 * NORM_BUT_WIDTH) / 2) - 2 * BUT_GAP, 
                POP_UP_EXIT_YES_BUT_CENTRE_Y = DEFAULT_WIN_HEIGHT - (NORM_BUT_HEIGHT / 2) - BUT_GAP, POP_UP_EXIT_YES_BUT = 5};
enum popUpExitNoButton{POP_UP_EXIT_NO_BUT_CENTRE_X = DEFAULT_WIN_WIDTH - (NORM_BUT_WIDTH / 2) - BUT_GAP, 
                POP_UP_EXIT_NO_BUT_CENTRE_Y = DEFAULT_WIN_HEIGHT - (NORM_BUT_HEIGHT / 2) - BUT_GAP, POP_UP_EXIT_NO_BUT = 6};
enum popUpOkButton{POP_UP_OK_BUT_CENTRE_X = DEFAULT_WIN_WIDTH - (NORM_BUT_WIDTH / 2) - BUT_GAP, 
                POP_UP_OK_BUT_CENTRE_Y = DEFAULT_WIN_HEIGHT - (NORM_BUT_HEIGHT / 2) - BUT_GAP, POP_UP_OK_BUT = 7};

enum textFieldButton{TEXT_BUT_HEIGHT = 30, TEXT_BUT_WIDTH = 250, TEXT_BUT_TYPE = 2, TEXT_BUT_BORDER = 2};
enum textFieldOneButton{TEXT_ONE_BUT_CENTRE_X = (TEXT_BUT_WIDTH / 2) + 14 * BUT_GAP, TEXT_ONE_BUT_CENTRE_Y = (TEXT_BUT_HEIGHT / 2) + BUT_GAP};
enum textFieldTwoButton{TEXT_TWO_BUT_CENTRE_X = TEXT_ONE_BUT_CENTRE_X, TEXT_TWO_BUT_CENTRE_Y = TEXT_ONE_BUT_CENTRE_Y + TEXT_BUT_HEIGHT + BUT_GAP};
enum textFieldThreeButton{TEXT_THREE_BUT_CENTRE_X = TEXT_TWO_BUT_CENTRE_X, TEXT_THREE_BUT_CENTRE_Y = TEXT_TWO_BUT_CENTRE_Y + TEXT_BUT_HEIGHT + BUT_GAP};
enum textPos{TCP_IP_TEXT_POS_X = BUT_GAP, 
             TCP_IP_TEXT_POS_Y = (TEXT_BUT_HEIGHT / 2) + (3 * BUT_GAP) / 2, 
             PORT_TEXT_POS_X = BUT_GAP,
             PORT_TEXT_POS_Y = TEXT_ONE_BUT_CENTRE_Y + TEXT_BUT_HEIGHT + (3 * BUT_GAP) / 2,
             FILE_SEL_TEXT_POS_X = BUT_GAP, 
             FILE_SEL_TEXT_POS_Y = TEXT_TWO_BUT_CENTRE_Y + TEXT_BUT_HEIGHT + (3 * BUT_GAP) / 2};

#define TCP_IP_TEXT "TCP/IP address:"
#define PORT_TEXT "TCP/IP port no:"
#define FILE_SEL_TEXT "File to upload:"

/* Text Field Colours */
#define TEXT_BUT_COLOUR_R 1.0
#define TEXT_BUT_COLOUR_G 1.0
#define TEXT_BUT_COLOUR_B 1.0
#define TEXT_BUT_TEXT_R 0
#define TEXT_BUT_TEXT_G 0
#define TEXT_BUT_TEXT_B 0
#define TEXT_BORDER_COLOUR_R 0.5
#define TEXT_BORDER_COLOUR_G 0.5
#define TEXT_BORDER_COLOUR_B 0.5
#define TEXT_GAP 5
#define PORT_NUM_BUT 2
#define TCP_IP_BUT 3
#define DIR_BUT 4
#define BUT_FONT GLUT_BITMAP_8_BY_13

/* Background Colour */
#define BKGD_R 0.8
#define BKGD_G 0.8
#define BKGD_B 0.8

/* Button Colour */
#define BUT_COLOUR_R 0
#define BUT_COLOUR_G 0.3
#define BUT_COLOUR_B 0.3

#define LIGHT_BUT_DIM 0.9
#define BUT_DIM 0.6

/* Button Text Colour */
#define BUT_TEXT_R 1.0
#define BUT_TEXT_G 1.0
#define BUT_TEXT_B 1.0

/* Button Border Colour */
#define BORDER_COLOUR_R 0.5
#define BORDER_COLOUR_G 0.5
#define BORDER_COLOUR_B 0.5

#define NUM_BUTTONS 8
#define MOUSE_NOT_IN_BUTTON -1

#define KEY_PRESSED_STR_SIZE 2
#define MAX_TEXT_BUT_STRLEN 256

/********************************************************************************************************
*                        Global to File Variables
********************************************************************************************************/
/* Used to implementing the main menu and end-game buttons. These buttons are required when user input is needed to decide
 * the course of the program/ game. */
static button_t buttonMatrix[NUM_BUTTONS];

static int mainWindow;
static int popUpWindow;

/********************************************************************************************************
*                        Public Functions
********************************************************************************************************/
/* Display Settings */
void initGlutOpenglSettings(void); /* More GLUT/ openGL initialisations */
void resizeWindow(int windowWidth, int windowHeight); /* Keeps window at a size of 1000x800 */

/* Game Menu Functions */
void createButtons(void);

/* Button Display Function (Menus)  */
void drawButton(button_t *button);
void drawButtons(void);

/* Mouse Related Functions */
static void mouseClickOnButtons(int mouseButton, int mouseButtonState, int mousePositionX, int mousePositionY);
static void mouseMotionWithButtons(int mousePositionX, int mousePositionY);
static int mouseInAnyButton(int mousePositionX, int mousePositionY);
static int mouseInButton(int mousePositionX, int mousePositionY, button_t *button);

/* Mouse Callback Functions */
void mouseButtonFunc(int mouseButton, int mouseButtonState, int mousePositionX, int mousePositionY);
void mouseMotionFunc(int mousePositionX, int mousePositionY); 
void passiveMotionFunc(int mousePositionX, int mousePositionY);

/* Get/ Change Button ... */
void changeButtonState(button_t *button, int newButtonState);
int getButtonState(button_t *button);
int getButtonPositionX(button_t *button);
int getButtonPositionY(button_t *button);

/* Get Button Edges */
int getLeftButtonEdge(button_t *button);
int getRightButtonEdge(button_t *button);
int getTopButtonEdge(button_t *button);
int getBottomButtonEdge(button_t *button);

/*  General Display Function (Menus and In-Game Screen) */
void mainDisplay(void);

/* Text Rendering Functions */
/* renderText is used to draw general bitmap messages to the screen */
static void renderText(int stringPosX, int stringPosY, void *font, char *string);

/* renderButtonText draws a bitmap text to the screen depending on the position of a button */  
static void renderButtonText(void *font, button_t *button); 

/* Pop-Up Functions */
void popUpExitDisplay (void);
void drawExitPopUpButtons(void);

void normalKeyboardFunc(unsigned char keyPressed, int mousePositionX, int mousePositionY);
void createPopUpWindow(char *windowName, void (*popUpWindowDisplayFunc)(void));
void closePopUpWindow(void);


void mousePopUpButtonFunc(int mouseButton, int mouseButtonState, int mousePositionX, int mousePositionY); 
void mousePopUpMotionFunc(int mousePositionX, int mousePositionY); 
void passivePopUpMotionFunc(int mousePositionX, int mousePositionY);

/********************************************************************************************************
 * 
 *                        Text Rendering Functions   
 * 
 * ******************************************************************************************************/

static void renderText(int stringPosX, int stringPosY, void *font, char *string)
{   
    char *stringChar; /* Used as a pointer to a character in string */    
    
    glRasterPos2i(stringPosX, stringPosY); /* Positioning text */
    for (stringChar = string; *stringChar != '\0'; stringChar++) /* Drawing a character at a time */
    {
        glutBitmapCharacter(font, *stringChar);
    }
}



static void renderButtonText(void *font, button_t *button)
{     
    int stringPosX = 0;
    int stringPosY = 0;
    char *buttonString; /* Used as a pointer to a character in buttonString */ 
    int numOfCharsThatFit = 0;
    int strLen = 0;
    int buttonLength = 0;
    
    buttonString = button->buttonString;
    if (button->buttonType == NORM_BUT_TYPE)
    {
        /* Figuring out where to position button text on screen */
        stringPosX = getButtonPositionX(button) - (glutBitmapLength(font, (const unsigned char *) buttonString) / 2);
        stringPosY = getButtonPositionY(button) + BUT_CENTRE_TEXT_Y;
    }
    else if (button->buttonType == TEXT_BUT_TYPE)
    {
        /* Figuring out where to position text field text on screen */
        stringPosX = getButtonPositionX(button) - ((button->buttonWidth) / 2) + TEXT_BUT_BORDER + TEXT_GAP;
        stringPosY = getButtonPositionY(button) + BUT_CENTRE_TEXT_Y;
        buttonLength = button->buttonWidth - 2 * TEXT_BUT_BORDER - 2 * TEXT_GAP;
        
        if (glutBitmapLength(BUT_FONT, (const unsigned char *)button->buttonString) >= buttonLength)
        {
            numOfCharsThatFit = buttonLength / glutBitmapWidth(font, 'm');
            strLen = strlen(button->buttonString) + 1;
            
            buttonString = button->buttonString + (strLen - 1) - numOfCharsThatFit;
        }
    }
    
    glColor3f(button->textColour[0], button->textColour[1], button->textColour[2]); /* Button character colour */
    glRasterPos2i(stringPosX, stringPosY); /* Positioning button text */
    
    while (*buttonString != '\0') /* Drawing a character at a time */
    {
        glutBitmapCharacter(font, *buttonString);
        
        buttonString++;
    }
}





/********************************************************************************************************
 * 
 *                        Button Display Function (Menus)  
 * 
 * ******************************************************************************************************/

void drawButton(button_t *button)
{
    /* Determine the edges of the button */
    int leftButtonEdge = getLeftButtonEdge(button), rightButtonEdge = getRightButtonEdge(button),
        topButtonEdge = getTopButtonEdge(button), bottomButtonEdge = getBottomButtonEdge(button);
    
    int buttonState = getButtonState(button);
    
    
    /* What state is the button in? (Deciding how to colour button) */
    switch(buttonState)
    {
       case BUT_IN: 
       case BUT_OVER:
       case BUT_PRESSED:
           glColor3f(button->borderColour[0] * BUT_DIM, button->borderColour[1] * BUT_DIM, button->borderColour[2] * BUT_DIM);
           break;
       case BUT_OUT:
           glColor3f(button->borderColour[0], button->borderColour[1], button->borderColour[2]); 
           break;
    }
    
    /* Draw border of button */
    glBegin(GL_QUADS);
    glVertex2f(leftButtonEdge, topButtonEdge); /* Top left corner */
    glVertex2f(rightButtonEdge, topButtonEdge); /* Top right corner */
    glVertex2f(rightButtonEdge, bottomButtonEdge); /* Bottom right corner */
    glVertex2f(leftButtonEdge, bottomButtonEdge); /* Bottom left corner */
    glEnd();
    
    /* What state is the button in? (Deciding how to colour button) */
    switch (buttonState)
    {
       case BUT_OUT:
           glColor3f(button->buttonColour[0], button->buttonColour[1], button->buttonColour[2]);
           break;
       case BUT_IN:
       case BUT_OVER:
           glColor3f(button->buttonColour[0] * LIGHT_BUT_DIM, button->buttonColour[1] * LIGHT_BUT_DIM, button->buttonColour[2] * LIGHT_BUT_DIM);
           break;
       case BUT_PRESSED:
           glColor3f(button->buttonColour[0] * BUT_DIM, button->buttonColour[1] * BUT_DIM, button->buttonColour[2] * BUT_DIM);
           break;
    }
    
    /* Draw the main part of the button */
    glBegin(GL_QUADS);
    
    if (button->buttonType == NORM_BUT_TYPE)
    {
        glVertex2f(leftButtonEdge + NORM_BUT_BORDER, topButtonEdge + NORM_BUT_BORDER); /* Top left corner */
        glVertex2f(rightButtonEdge - NORM_BUT_BORDER, topButtonEdge + NORM_BUT_BORDER); /* Top right corner */
        glVertex2f(rightButtonEdge - NORM_BUT_BORDER, bottomButtonEdge - NORM_BUT_BORDER); /* Bottom right corner */
        glVertex2f(leftButtonEdge + NORM_BUT_BORDER, bottomButtonEdge - NORM_BUT_BORDER); /* Bottom left corner */ 
    }
    else if (button->buttonType == TEXT_BUT_TYPE)
    {
        glVertex2f(leftButtonEdge + TEXT_BUT_BORDER, topButtonEdge + TEXT_BUT_BORDER); /* Top left corner */
        glVertex2f(rightButtonEdge - TEXT_BUT_BORDER, topButtonEdge + TEXT_BUT_BORDER); /* Top right corner */
        glVertex2f(rightButtonEdge - TEXT_BUT_BORDER, bottomButtonEdge - TEXT_BUT_BORDER); /* Bottom right corner */
        glVertex2f(leftButtonEdge + TEXT_BUT_BORDER, bottomButtonEdge - TEXT_BUT_BORDER); /* Bottom left corner */
    }
    
    glEnd();
    glPushMatrix();
    renderButtonText(BUT_FONT, button);
    glPopMatrix();
              
}



/********************************************************************************************************
 * 
 *                       Button Creation
 * 
 * ******************************************************************************************************/

void createButtons(void)
{
    int i = 0;
    
    //FIXME:NEED to free these 
    
    for (i = 0; i < NUM_BUTTONS; i++)
    {
        buttonMatrix[i].buttonState = BUT_OUT;
    }
    
    for (i = 0; i < 2; i++)//FIXME
    {
        buttonMatrix[i].textColour[0] = BUT_TEXT_R;
        buttonMatrix[i].textColour[1] = BUT_TEXT_G;
        buttonMatrix[i].textColour[2] = BUT_TEXT_B; 
        
        buttonMatrix[i].buttonColour[0] = BUT_COLOUR_R;
        buttonMatrix[i].buttonColour[1] = BUT_COLOUR_G;
        buttonMatrix[i].buttonColour[2] = BUT_COLOUR_B;
        
        buttonMatrix[i].borderColour[0] = BORDER_COLOUR_R;
        buttonMatrix[i].borderColour[1] = BORDER_COLOUR_G;
        buttonMatrix[i].borderColour[2] = BORDER_COLOUR_B;
    }
    
    for (i = 2; i < 5; i++)//FIXME
    {
        buttonMatrix[i].textColour[0] = TEXT_BUT_TEXT_R;
        buttonMatrix[i].textColour[1] = TEXT_BUT_TEXT_G;
        buttonMatrix[i].textColour[2] = TEXT_BUT_TEXT_B; 
        
        buttonMatrix[i].buttonColour[0] = TEXT_BUT_COLOUR_R;
        buttonMatrix[i].buttonColour[1] = TEXT_BUT_COLOUR_G;
        buttonMatrix[i].buttonColour[2] = TEXT_BUT_COLOUR_B;
        
        buttonMatrix[i].borderColour[0] = TEXT_BORDER_COLOUR_R;
        buttonMatrix[i].borderColour[1] = TEXT_BORDER_COLOUR_G;
        buttonMatrix[i].borderColour[2] = TEXT_BORDER_COLOUR_B;
        
        buttonMatrix[i].buttonString = malloc(MAX_TEXT_BUT_STRLEN * sizeof(char));
    }
    
    /* Setting up buttons attributes and button strings depending on current gameMode */
    buttonMatrix[0].buttonString = "Upload";
    buttonMatrix[0].buttonPositionX = UPLOAD_BUT_CENTRE_X;
    buttonMatrix[0].buttonPositionY = UPLOAD_BUT_CENTRE_Y;
    buttonMatrix[0].buttonType = NORM_BUT_TYPE;
    buttonMatrix[0].buttonHeight = NORM_BUT_HEIGHT;
    buttonMatrix[0].buttonWidth = NORM_BUT_WIDTH;  
    buttonMatrix[0].buttonFunc = UPLOAD_BUT;  
    
    buttonMatrix[1].buttonString = "Exit";
    buttonMatrix[1].buttonPositionX = EXIT_BUT_CENTRE_X;
    buttonMatrix[1].buttonPositionY = EXIT_BUT_CENTRE_Y;
    buttonMatrix[1].buttonType = NORM_BUT_TYPE;
    buttonMatrix[1].buttonHeight = NORM_BUT_HEIGHT;
    buttonMatrix[1].buttonWidth = NORM_BUT_WIDTH;
    buttonMatrix[1].buttonFunc = EXIT_BUT;  
    
    strcpy(buttonMatrix[2].buttonString, "");
    buttonMatrix[2].buttonPositionX = TEXT_ONE_BUT_CENTRE_X;
    buttonMatrix[2].buttonPositionY = TEXT_ONE_BUT_CENTRE_Y;
    buttonMatrix[2].buttonType = TEXT_BUT_TYPE;
    buttonMatrix[2].buttonHeight = TEXT_BUT_HEIGHT;
    buttonMatrix[2].buttonWidth = TEXT_BUT_WIDTH;
    buttonMatrix[2].buttonFunc = TCP_IP_BUT;  
    
    strcpy(buttonMatrix[3].buttonString, "");
    buttonMatrix[3].buttonPositionX = TEXT_TWO_BUT_CENTRE_X;
    buttonMatrix[3].buttonPositionY = TEXT_TWO_BUT_CENTRE_Y;
    buttonMatrix[3].buttonType = TEXT_BUT_TYPE;
    buttonMatrix[3].buttonHeight = TEXT_BUT_HEIGHT;
    buttonMatrix[3].buttonWidth = TEXT_BUT_WIDTH;
    buttonMatrix[3].buttonFunc = PORT_NUM_BUT;  
    
    strcpy(buttonMatrix[4].buttonString, "");
    buttonMatrix[4].buttonPositionX = TEXT_THREE_BUT_CENTRE_X;
    buttonMatrix[4].buttonPositionY = TEXT_THREE_BUT_CENTRE_Y;
    buttonMatrix[4].buttonType = TEXT_BUT_TYPE;
    buttonMatrix[4].buttonHeight = TEXT_BUT_HEIGHT;
    buttonMatrix[4].buttonWidth = TEXT_BUT_WIDTH;
    buttonMatrix[4].buttonFunc = DIR_BUT;  
    
    buttonMatrix[5].buttonString = "Yes";
    buttonMatrix[5].buttonPositionX = POP_UP_EXIT_YES_BUT_CENTRE_X;
    buttonMatrix[5].buttonPositionY = POP_UP_EXIT_YES_BUT_CENTRE_Y;
    buttonMatrix[5].buttonType = NORM_BUT_TYPE;
    buttonMatrix[5].buttonHeight = NORM_BUT_HEIGHT;
    buttonMatrix[5].buttonWidth = NORM_BUT_WIDTH;
    buttonMatrix[5].buttonFunc = POP_UP_EXIT_YES_BUT;  
    
    buttonMatrix[6].buttonString = "No";
    buttonMatrix[6].buttonPositionX = POP_UP_EXIT_NO_BUT_CENTRE_X;
    buttonMatrix[6].buttonPositionY = POP_UP_EXIT_NO_BUT_CENTRE_Y;
    buttonMatrix[6].buttonType = NORM_BUT_TYPE;
    buttonMatrix[6].buttonHeight = NORM_BUT_HEIGHT;
    buttonMatrix[6].buttonWidth = NORM_BUT_WIDTH;
    buttonMatrix[6].buttonFunc = POP_UP_EXIT_NO_BUT; 
    
    buttonMatrix[7].buttonString = "Ok";
    buttonMatrix[7].buttonPositionX = POP_UP_OK_BUT_CENTRE_X;
    buttonMatrix[7].buttonPositionY = POP_UP_OK_BUT_CENTRE_Y;
    buttonMatrix[7].buttonType = NORM_BUT_TYPE;
    buttonMatrix[7].buttonHeight = NORM_BUT_HEIGHT;
    buttonMatrix[7].buttonWidth = NORM_BUT_WIDTH;
    buttonMatrix[7].buttonFunc = POP_UP_OK_BUT; 
    
    for (i = 5; i < NUM_BUTTONS; i++)//FIXME
    {
        buttonMatrix[i].textColour[0] = BUT_TEXT_R;
        buttonMatrix[i].textColour[1] = BUT_TEXT_G;
        buttonMatrix[i].textColour[2] = BUT_TEXT_B; 
        
        buttonMatrix[i].buttonColour[0] = BUT_COLOUR_R;
        buttonMatrix[i].buttonColour[1] = BUT_COLOUR_G;
        buttonMatrix[i].buttonColour[2] = BUT_COLOUR_B;
        
        buttonMatrix[i].borderColour[0] = BORDER_COLOUR_R;
        buttonMatrix[i].borderColour[1] = BORDER_COLOUR_G;
        buttonMatrix[i].borderColour[2] = BORDER_COLOUR_B;
    }
    
}

void drawButtons(void)
{
    int buttonIndex = 0;
    
    /* Draw all buttons */
    for (buttonIndex = 0; buttonIndex < NUM_BUTTONS; buttonIndex++) 
    {  
       if ((buttonMatrix[buttonIndex].buttonFunc == UPLOAD_BUT)   || 
           (buttonMatrix[buttonIndex].buttonFunc == EXIT_BUT)     || 
           (buttonMatrix[buttonIndex].buttonFunc == TCP_IP_BUT)   || 
           (buttonMatrix[buttonIndex].buttonFunc == PORT_NUM_BUT) || 
           (buttonMatrix[buttonIndex].buttonFunc == DIR_BUT))
       {
           drawButton(&buttonMatrix[buttonIndex]);
       }
    }
}

void drawExitPopUpButtons(void)
{
    int buttonIndex = 0;
    
    /* Draw all buttons */
    for (buttonIndex = 0; buttonIndex < NUM_BUTTONS; buttonIndex++) 
    {  
       if ((buttonMatrix[buttonIndex].buttonFunc == POP_UP_EXIT_YES_BUT) || 
           (buttonMatrix[buttonIndex].buttonFunc == POP_UP_EXIT_NO_BUT))
       {
           drawButton(&buttonMatrix[buttonIndex]);
       }
    }
}



/********************************************************************************************************
 * 
 *                        General Display Function (Menus and In-Game Screen)
 * 
 * ******************************************************************************************************/

/* GLUT call-back function. Used to display menus and in-game content */
void mainDisplay(void)
{    
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawButtons();
    
    glPushMatrix();
    glColor3f(TEXT_BUT_TEXT_R, TEXT_BUT_TEXT_G, TEXT_BUT_TEXT_B);
    renderText(TCP_IP_TEXT_POS_X, TCP_IP_TEXT_POS_Y, BUT_FONT, TCP_IP_TEXT); /* Player 1's Score Highlighted */
    glColor3f(TEXT_BUT_TEXT_R, TEXT_BUT_TEXT_G, TEXT_BUT_TEXT_B);
    renderText(PORT_TEXT_POS_X, PORT_TEXT_POS_Y, BUT_FONT, PORT_TEXT); /* Player 1's Score Highlighted */
    glColor3f(TEXT_BUT_TEXT_R, TEXT_BUT_TEXT_G, TEXT_BUT_TEXT_B);
    renderText(FILE_SEL_TEXT_POS_X, FILE_SEL_TEXT_POS_Y, BUT_FONT, FILE_SEL_TEXT); /* Player 1's Score Highlighted */
    glPopMatrix();
    
    glutSwapBuffers();
}

void popUpExitDisplay (void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawExitPopUpButtons();
    
    glutSwapBuffers();
}

/********************************************************************************************************
 * 
 *                        Display Settings  
 * 
 * ******************************************************************************************************/

/* More GLUT/ openGL initialisations */
void initGlutOpenglSettings(void)
{
    /* Setting background colour */
    glClearColor(BKGD_R, BKGD_G, BKGD_B, 0); 
    
    /* Setting the origin of the viewing area at top left corner. Order is (Left,Right,Bottom,Top). */
    gluOrtho2D(0, DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT, 0); 
}


/* GLUT call-back function. Keeps window at the same size throughout the programs life */
void resizeWindow(int windowWidth, int windowHeight)
{
    glutReshapeWindow(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT);
}

void refreshDisplay(void)
{
    glutPostRedisplay();
}




/********************************************************************************************************
 * 
 *                       Mouse Callback Functions
 * 
 * ******************************************************************************************************/

/* GLUT call-back function which handles only mouse clicks */
void mouseButtonFunc(int mouseButton, int mouseButtonState, int mousePositionX, int mousePositionY) 
{
    
        mouseClickOnButtons(mouseButton, mouseButtonState, mousePositionX, mousePositionY);

//     else if (gameMode == MOUSE_DISABLE)
//     {
//         /* Do nothing...*/
//     }
}

/* GLUT call-back function which handles both mouse movements and mouse clicks together */
void mouseMotionFunc(int mousePositionX, int mousePositionY) 
{

        mouseMotionWithButtons(mousePositionX, mousePositionY);

//     else if (gameMode == MOUSE_DISABLE)
//     {
//         /* Do nothing...*/
//     }
}

/* GLUT call-back function which handles only mouse movements */
void passiveMotionFunc(int mousePositionX, int mousePositionY)
/* Handles situations where a mouse button(s) is not pressed before entering the area of a card. Mouse cursor is just dragged on card */
{
    mouseMotionFunc(mousePositionX, mousePositionY); 
}














/* GLUT call-back function which handles only mouse clicks */
void mousePopUpButtonFunc(int mouseButton, int mouseButtonState, int mousePositionX, int mousePositionY) 
{
    
        mouseClickOnButtons(mouseButton, mouseButtonState, mousePositionX, mousePositionY);

//     else if (gameMode == MOUSE_DISABLE)
//     {
//         /* Do nothing...*/
//     }
}

/* GLUT call-back function which handles both mouse movements and mouse clicks together */
void mousePopUpMotionFunc(int mousePositionX, int mousePositionY) 
{

        mouseMotionWithButtons(mousePositionX, mousePositionY);

//     else if (gameMode == MOUSE_DISABLE)
//     {
//         /* Do nothing...*/
//     }
}

/* GLUT call-back function which handles only mouse movements */
void passivePopUpMotionFunc(int mousePositionX, int mousePositionY)
/* Handles situations where a mouse button(s) is not pressed before entering the area of a card. Mouse cursor is just dragged on card */
{
    mousePopUpMotionFunc(mousePositionX, mousePositionY); 
}

/********************************************************************************************************
 * 
 *                       Mouse Related Functions
 * 
 * ******************************************************************************************************/

static void mouseClickOnButtons(int mouseButton, int mouseButtonState, int mousePositionX, int mousePositionY)
{
    int buttonIndex;
    button_t *button;
    int i;
    
    /* Is mouse click with left mouse button and within a displayed button? */
    if ( ((buttonIndex = mouseInAnyButton(mousePositionX, mousePositionY)) > MOUSE_NOT_IN_BUTTON) && (mouseButton == GLUT_LEFT_BUTTON) ) 
    {
        button = &buttonMatrix[buttonIndex]; 
        
        /* Change the buttonState of the 'physical' button that was clicked on */ 
        switch (getButtonState(button))
        {
            case BUT_OUT:
            case BUT_OVER:
                if (mouseButtonState == GLUT_DOWN) /* Mouse button is held down */
                {
                    changeButtonState(button, BUT_PRESSED);                       
                    refreshDisplay(); 
                }
                break;
            case BUT_PRESSED:
                if (mouseButtonState == GLUT_UP) /* Mouse button has been released */
                {
                    changeButtonState(button, BUT_IN); 
                    
                    for (i = 0; i < NUM_BUTTONS; i++) //Deselect all other textfields
                    {
                        if ((buttonMatrix[i].buttonType == TEXT_BUT_TYPE) && (i != buttonIndex))
                        {
                            changeButtonState(&buttonMatrix[i], BUT_OUT); 
                        }
                    }
                    
//                     if (button->buttonType == NORM_BUT_TYPE)
//                     {
//                         switch (button->buttonFunc)
//                         {
//                             case UPLOAD_BUT:
//                                 break;
//                             case EXIT_BUT:
//                                 break;
//                         }
//                     }
//                     else if (button->buttonType == TEXT_BUT_TYPE)
//                     {
//                         switch (button->buttonFunc)
//                         {
//                             case TCP_IP_BUT:
//                                 break;
//                             case PORT_NUM_BUT:
//                                 break;
//                             case DIR_BUT:
//                                 break;
//                         }
//                     }
                    
                    refreshDisplay(); 
                }
                break;
        }
    }
}



/* Functions for mouse motion actions acting on 'physical' buttons */
static void mouseMotionWithButtons(int mousePositionX, int mousePositionY)
{
    int buttonIndex;
    button_t *button;
    int resetButtonIndex; /* Variable required to reset all previously changed button buttonStates back to the default BUT_OUT */
    
    /* Is mouse cursor within a button? */
    if ((buttonIndex = mouseInAnyButton(mousePositionX, mousePositionY)) > MOUSE_NOT_IN_BUTTON)
    {
        button = &buttonMatrix[buttonIndex];
        switch (getButtonState(button))
        {
            case BUT_OUT:
                changeButtonState(button, BUT_OVER);
                refreshDisplay();    
                break;
        }
        
        for (resetButtonIndex = 0; resetButtonIndex < NUM_BUTTONS; resetButtonIndex++) /* Removes a multiple button highlight glitch which occur with rapid cursor movements */
                if (((buttonMatrix[resetButtonIndex].buttonState == BUT_OVER) || (buttonMatrix[resetButtonIndex].buttonState == BUT_PRESSED)) 
                    && (resetButtonIndex != buttonIndex))
                {
                    changeButtonState(&buttonMatrix[resetButtonIndex], BUT_OUT); 
                }           
        refreshDisplay();//FIXME I changed this!!!!!!!   
                
    }
    else  /* Mouse cursor outside all buttons. We must ensure all buttons have state BUT_OUT */
        { 
            for (buttonIndex = 0; buttonIndex < NUM_BUTTONS; buttonIndex++)
                if ((buttonMatrix[buttonIndex].buttonState == BUT_OVER) || (buttonMatrix[buttonIndex].buttonState == BUT_PRESSED))
                {
                    changeButtonState(&buttonMatrix[buttonIndex], BUT_OUT);
                    refreshDisplay();    
                }
        }
}

/* Determines if any buttons had an left mouse button action act upon it */
static int mouseInAnyButton(int mousePositionX, int mousePositionY)
{
   int buttonIndex;
   
   for (buttonIndex = 0; buttonIndex < NUM_BUTTONS; buttonIndex++)
   {
      if (mouseInButton(mousePositionX, mousePositionY, &buttonMatrix[buttonIndex]))
         return buttonIndex;
   }
   return -1;
}

/* Checking if left mouse button action occurs within a particular button */
static int mouseInButton(int mousePositionX, int mousePositionY, button_t *button)
{
   int leftButtonEdge = getLeftButtonEdge(button), rightButtonEdge = getRightButtonEdge(button),
       topButtonEdge = getTopButtonEdge(button), bottomButtonEdge = getBottomButtonEdge(button);
       
   return (mousePositionX >= leftButtonEdge && mousePositionX <= rightButtonEdge
        && mousePositionY >= topButtonEdge  && mousePositionY <= bottomButtonEdge);
}



/********************************************************************************************************
 * 
 *                       Get and Change Functions
 * 
 * ******************************************************************************************************/

void changeButtonState(button_t *button, int newButtonState)
{
   button->buttonState = newButtonState;
}



int getButtonState(button_t *button)
{
   return button->buttonState;
}



int getButtonPositionX(button_t *button)
{
    return button->buttonPositionX;
}



int getButtonPositionY(button_t *button)
{
    return button->buttonPositionY;
}


int getLeftButtonEdge(button_t *button)
{
    return (button->buttonPositionX - (button->buttonWidth) / 2);
}



int getRightButtonEdge(button_t *button)
{
    return (button->buttonPositionX + (button->buttonWidth) / 2);
}



int getTopButtonEdge(button_t *button)
{
    return (button->buttonPositionY - (button->buttonHeight) / 2);
}



int getBottomButtonEdge(button_t *button)
{
    return (button->buttonPositionY + (button->buttonHeight) / 2);
}



/********************************************************************************************************
 * 
 *                       Keyboard Related Functions
 * 
 * ******************************************************************************************************/
void normalKeyboardFunc(unsigned char keyPressed, int mousePositionX, int mousePositionY)
{
    int i = 0;
    char keyPressedStr[KEY_PRESSED_STR_SIZE];
    char *buttonStrCharPtr;
    //char *buttonString; 
    
    sprintf(keyPressedStr, "%c", keyPressed);
    for (i = 0; i < NUM_BUTTONS; i++)
    {
        if ((getButtonState(&buttonMatrix[i]) == BUT_IN) && (buttonMatrix[i].buttonType == TEXT_BUT_TYPE))
        {
            if (keyPressed == 0xD) //carriage return (LF)
            {
                changeButtonState(&buttonMatrix[i], BUT_OUT);
                //TODO:send the string for analysis buttonMatrix[i].buttonString
            }
            else if (keyPressed == 0x8) //backspace
            {
                buttonStrCharPtr = buttonMatrix[i].buttonString + (strlen(buttonMatrix[i].buttonString) - 1); //final char. -1 for array access.note:strlen excludes null char
                *buttonStrCharPtr = '\0';                
            }
            else if ((keyPressed == 0x9) || (keyPressed == 0x1B)) //tab or esc
            {
                changeButtonState(&buttonMatrix[i], BUT_OUT);
                //do nothing, user doesn't want to enter any more or enter
            }
        //      else if (keyPressed == 0x7F) //del//FIXME:NEED arrow shifting keys for this.can just ignore too complicated
        //      {
            //          
            //      }
            else if (((keyPressed >= ' ') || (keyPressed <= '~')) && ((strlen(buttonMatrix[i].buttonString) + 1) < MAX_TEXT_BUT_STRLEN)) 
            {
                //buttonString = buttonMatrix[i].buttonString;                   
                    
                strcat(buttonMatrix[i].buttonString, keyPressedStr);                   
            }
        }
    }
    
    

    
    glutPostRedisplay();
}


/********************************************************************************************************
 * 
 *                       Pop-Up Window Related Functions
 * 
 * ******************************************************************************************************/
void createPopUpWindow(char *windowName, void (*popUpWindowDisplayFunc)(void))
{
    glutInitWindowSize(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT); 
    glutInitWindowPosition(DEFAULT_WIN_POSITION_X, DEFAULT_WIN_POSITION_Y);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    popUpWindow = glutCreateWindow(windowName);
    initGlutOpenglSettings(); 
    
    /* Registering Callback Functions */
    glutDisplayFunc(popUpWindowDisplayFunc);
    glutMouseFunc(mousePopUpButtonFunc);
    glutMotionFunc(mousePopUpMotionFunc);
    glutPassiveMotionFunc(passivePopUpMotionFunc);
    glutReshapeFunc(resizeWindow); /* Removes the distortion caused by rescaling the window */       
}

void closePopUpWindow(void)
{
    
    glutDestroyWindow(popUpWindow);   
}




int main(int argc, char **argv)
{
    /* Initialise Buttons */
    createButtons();
    
    /* GLUT/openGL Initialisations for Window */
    glutInit(&argc, argv);
    glutInitWindowSize(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT); 
    glutInitWindowPosition(DEFAULT_WIN_POSITION_X, DEFAULT_WIN_POSITION_Y);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    mainWindow = glutCreateWindow("ftclient");
    initGlutOpenglSettings(); 
    
    /* Registering Callback Functions */
    glutDisplayFunc(mainDisplay);
    glutMouseFunc(mouseButtonFunc);
    glutMotionFunc(mouseMotionFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutKeyboardFunc(normalKeyboardFunc);
    glutReshapeFunc(resizeWindow); /* Removes the distortion caused by rescaling the window */    
    
//      createPopUpWindow("ERROR", popUpExitDisplay);
//      closePopUpWindow();
    /* Entering the GLUT event processing loop */
    glutMainLoop();
    return 0;
    
}