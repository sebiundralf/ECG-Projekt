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

#include "tga.h"
#include "ECG-Memory/class_Memory.h"

int window; 
float rotation_x, rotation_z;
GLuint texture;
GLuint texture2;
const int cards = 2;
float rotation_y[cards];
int animating[cards];
float rotation_progress[cards];
int found[cards];
float rotating_speed = 5;
float q = 0.0;
float w = 0.5;
float e = 0.2;
float r = 1.0;
int mousex = 0;
int mousey = 0;
int mousePress = 0;

int temp[cards];


void reportGLError(const char * msg)
{
  GLenum errCode;
  const GLubyte *errString;
  while ((errCode = glGetError()) != GL_NO_ERROR) {
    errString = gluErrorString(errCode);
    fprintf(stderr, "OpenGL Error: %s %s\n", msg, errString);
  }
  return;
}

void resize(int width, int height)
{
  // prevent division by zero
  if (height == 0) { height=1; }

  glViewport(0, 0, width, height);  

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (float)width/(float)height, 0.01f, 10.0f);
  glMatrixMode(GL_MODELVIEW);
}

void keyPressed(unsigned char key, int x, int y) 
{
 // usleep(100);
  switch (key) {
  case 27:
    glutDestroyWindow(window); 
    exit(0);                   
    break;
  case 'a':
	  animating[0] = animating[1] = 1;
    glutPostRedisplay();
    break;
  /*case 'r':
    //rotation_y++;
    if (rotation_y >= 360) {
      rotation_y -= 360.0;
    }
    glutPostRedisplay();

    break;*/
  case '2':
	   rotating_speed = 2;
	  break;
  case '5':
	   rotating_speed = 5;
	  break;
  case '0':
	   rotating_speed = 20;
	  break;
  case 'f':
	  glutPostRedisplay();
  case 'q':
	  if(q==0)
		  q = 10;
	  else
		  q = 0;
	  break;
  case 'w':
	  if(w==0.5)
		  w = 5.0;
	  else
		  w = 0.5;
	break;

  case 'e':
	  if(e==0.2)
		  e = 10;
	  else 
		  e = 0.2;
  case 'r':
	  if(r == 1.0)
		  r = 10;
	  else
		  r=1.0;
	  break;
  default:
    break;
  }
}
void drawCard(){

	  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	//glColor4f(1.0,1.0,1.0,0);

  // front face
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  0.01f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  0.01f);
  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.01f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  0.01f);
  
  glEnd();
  
  glDisable(GL_TEXTURE_2D);
  
    glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, texture2);

	glBegin(GL_QUADS);
  
  // back face
  
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -0.00f);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -0.00f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -0.00f);
  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -0.00f);

    glEnd();
  
  glDisable(GL_TEXTURE_2D);



}
void display()
{
	GLfloat light_ambient[] = { q, w, e , r};
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
  glLoadIdentity();  


  
  glTranslatef(0, 0, -6);
	
  glPushMatrix();
  	
	glTranslatef(0,0,-1);
	glColor4f(0.9,1.0,0.8,0.9);

	glBegin(GL_QUADS);
  // front face
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, -10.0f,  0.01f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, -10.0f,  0.01f);
  glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f,  10.00f,  0.01f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f,  10.0f,  0.01f);
  
  glEnd();
  glPopMatrix();
 // glRotatef(rotation_x, 1, 0, 0);
 // glRotatef(rotation_y, 0, 1, 0);
//  glRotatef(rotation_z, 0, 0, 1);

  glLightfv(GL_LIGHT0, GL_SPECULAR, light_ambient);
  glEnable(GL_LIGHT0);


  for(int i = 0; i < 2; ++i){
	  
	    glPushMatrix();
		
	  switch(i){
	  case 0:  glTranslatef(1.5, 0, 0);
	
		  break;
	  case 1:
			glTranslatef(-1.5, 0, 0);
		   
			break;
	  default: 
		  printf("ERROR IN SWITCH_CASE\n");
		  break;
		   

	  }

	 glRotatef(rotation_y[i], 0, 1, 0);
	/*
	  glEnable (GL_BLEND);
    glBlendFunc (GL_ONE, GL_SRC_COLOR);
	*/
	 if(found[i]==0)
		drawCard();


  glPopMatrix();

  if (animating[i]) {
	  if(rotation_progress[i] == 180)
	  temp[i] = (temp[i])?0:1;
    //rotation_x += 0.2f;
	if(rotation_progress[i] > 180){
		printf("ERROR, ROTATION: %d \n", rotation_progress[i]);
		if(rotation_y[i] >90 || rotation_y[i]<270)
			rotation_y[i] = 180;
		else
			rotation_y[i] = 0;

	}

	if(rotation_progress[i] >= 180){
		rotation_progress[i] = animating[i] = 0;
	}else{
		rotation_y[i] += rotating_speed;
		rotation_progress[i] += rotating_speed;

	}
	 }
    
    //rotation_z += 0.1f;
    //glutPostRedisplay();
  }
  if((temp[0] + temp[1] )==2)
	  found[0]=found[1]=1;

  glutSwapBuffers();
}

void init(int width, int height)  
{
	FILE* file;
  GLsizei w, h;
  tgaInfo *info = 0;
  tgaInfo *info_back = 0;
  int mode;
  
  GLint format;
    char pfad[256]; //hier wird der Programmpfad gespeichert
  rotation_x = rotation_z = 0.0;
  
  for(int i = 0; i < cards; i++)
		 rotation_y[i] = animating[i] = rotation_progress[i] =found[i] = temp[i]= 0;
  
  
  
  


  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0);        
  glDepthFunc(GL_LESS);      
  glEnable(GL_DEPTH_TEST);  
  glShadeModel(GL_SMOOTH);  

  resize(width, height);




  /*getcwd(pfad, 256); //der Programmpfad ist jetzt in 'pfad' gespeichert

  printf("Pfad: %s\n", pfad);
  */

  info = tgaLoad("Cardback.tga");

  if (info->status != TGA_OK) {
    fprintf(stderr, "error loading texture image: %d\n", info->status);
    
    return;
  }

    info_back = tgaLoad("Cardfront.tga");

  if (info_back->status != TGA_OK) {
    fprintf(stderr, "error loading texture image: %d\n", info_back->status);
    
    return;
  }

  printf("CardBack: width: %d, height: %d\n, CardFront: width: %d, height: %d\n", info->width, info->height, info_back->width, info_back->height);

  if (info->width != info->height) {
    fprintf(stderr, "Image size %d x %d is not rectangular, giving up.\n",
            info->width, info->height);
    return;
  }

  mode = info->pixelDepth / 8;  // will be 3 for rgb, 4 for rgba
  glGenTextures(1, &texture);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  // Upload the texture bitmap. 
  w  = info->width; 
  h = info->height; 

  reportGLError("before uploading texture");
 format = (mode == 4) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, 
               GL_UNSIGNED_BYTE, info->imageData);
  reportGLError("after uploading texture");

  tgaDestroy(info);

  /*****************************************************************/


  
  mode = info_back->pixelDepth / 8;  // will be 3 for rgb, 4 for rgba
  glGenTextures(1, &texture2);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  // Upload the texture bitmap. 
  w  = info_back->width; 
  h = info_back->height; 

  reportGLError("before uploading texture");
 format = (mode == 4) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, 
               GL_UNSIGNED_BYTE, info_back->imageData);
  reportGLError("after uploading texture");

  tgaDestroy(info_back);


}

void timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(15, timer, 1);
}

void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:    /* spin scene around */
    if (state == GLUT_DOWN){
      
		if(x>75 && x<265 && y > 145 && y < 335){
			mousePress = 2;
		 
		}
		//printf("mouse x = %d, mouse y = %d\n", x, y);
		if(x>370 && x<560 && y > 145 && y < 335){
			mousePress = 1;

		}

    } else if (state == GLUT_UP){
		//printf("Mouse Press = %d\n",mousePress);
		if(mousePress!=0)
			animating[mousePress-1] = 1;
		mousePress=0;


		
    }
  }
  
  glutPostRedisplay();
}



int main1(int argc, char **argv) 
{  
  glutInit(&argc, argv);  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
  glutInitWindowSize(640, 480);  
  glutInitWindowPosition(0, 0);  
  window = glutCreateWindow("foo");  
  glutDisplayFunc(&display);  
  glutReshapeFunc(&resize);
  glutKeyboardFunc(&keyPressed);
  init(640, 480);
  glutMouseFunc(mouse);
  glutTimerFunc(15, timer, 1);

  glutMainLoop();  





  return 0;
}
