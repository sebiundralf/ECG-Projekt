#pragma once
#ifndef MEMORY_OPENGL_GLOBALS
#define MEMORY_OPENGL_GLOBALS


#include "class_Memory.h"
#include <stdlib.h> // for exit
//#include <unistd.h> // for usleep
#include <stdio.h>
#include <direct.h>



#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h> 
#include <OpenGL/gl.h>  
#include <OpenGL/glu.h>  
#else
#include <GL/glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  
#endif

#include "class_Memory.h"

#include "class_openGL.h"


 const int cards = 16;

class class_openGL{

 void vInitialize();
 static void drawBorder(float cardsize);

 public: 
 class_openGL(Memory * mem);


 static void keyPressed(unsigned char key, int x, int y);
 static void drawCard(int background, int x, int y);


 /* Glut Funktionen */
 static void init(int width, int height) ;
 static void display();
 static void timer(int value);
 static void mouse(int button, int state, int x, int y);
 static void start(int argc, char **argv);
 static void mouseMotion(int x, int y);
 static void reportGLError(const char * msg);
 static void resize(int width, int height);

};



#endif