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



int main1(int argc, char **argv) 
{  
	int sv = 0;
	printf("Difficulty? 0 1 or 2:  ");
	scanf("%d", &sv);
	difficulty dif;
	switch(sv){
	case 0: 
		dif = EASY;
		break;
	case 1: 
		dif =  MEDIUM;
		break;
	case 2: 
		dif = HARD;
		break;
	default:
		dif =  EASY;
		break;

	}


	Memory *mem = new Memory(dif);
	class_openGL op = class_openGL(mem);

	op.start(argc,argv);



	delete(mem);



  return 0;
}
