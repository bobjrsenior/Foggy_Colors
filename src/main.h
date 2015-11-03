/*
* @file main.h
* @brief Function prototypes and includes for main.c
*
*/

//Includes
//Include GLEW  
#include <GL/glew.h>  

//Include GLFW  
#include <GLFW/glfw3.h>
#include "matrixMath.h"


// Image importing
#include <SOIL.h>

//Include the standard C++ headers  
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
//Audio
#ifdef _WIN32
#include <Windows.h>
#include <Mmsystem.h>
#endif

//Draws text to the screen at a specified location and scale
void drawText(char* text, GLuint squareuniOverrideTex, GLuint squareuniShowTex, GLuint squareShaderProgram, GLuint squareuniTexOverride, GLuint squareuniScale, GLuint squareuniTranslation, GLuint squareuniColor, GLfloat xPos, GLfloat yPos, GLfloat scale);

//Callbacks

static void error_callback(int error, const char* description);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
