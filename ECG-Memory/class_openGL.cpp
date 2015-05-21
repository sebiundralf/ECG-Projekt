#include <stdlib.h> // for exit
//#include <unistd.h> // for usleep
#include <stdio.h>
#include <direct.h>
#include <string>
#include <iostream>

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
#include "tga.h"



static int window; 

static float rotation_x, rotation_z;
static  GLuint cardback;
static  GLuint cardfront[8];
const int x_lenght = 2;
const int y_lenght = 3;

static  int** animating;
static  int** found;

static float** rotation_y;
static  float** rotation_progress;
static int** temp;

static difficulty dif;

static   int rotating_speed;
static int q;
static  int w;
static int e ;
static int r;
static int mousex;
static int mousey;
static int mousePress;



float factor (0.8);
static float coordsHARD[4]= {-3*factor,-1*factor,1*factor,3*factor};
static Memory * memObj;





class_openGL::class_openGL(Memory*mem)
{
	rotating_speed = 5;
	q = 0.0;
	w = 0.5;
	e = 0.2;
	r = 1.0;
	mousex = 0;
	mousey = 0;
	mousePress = 0;
	memObj = mem;
	dif = mem->getCurrentDif();
	vInitialize();



}

void class_openGL::vInitialize(){
	for(int i = 0; i < 5; i++){

		int h = memObj->getHeight();
		int w = memObj->getWidth();
				int**temp1;
				float**temp2;
	
		if(i<3){
			temp1 = (int**) malloc(sizeof(int*) *h);
			for(int j = 0; j < 4; j++)
				temp1[j] = (int*) malloc(sizeof(int) *w);
			}else {		
			temp2 = (float**) malloc(sizeof(float*) *h);
			for(int j = 0; j < 4; j++)
				temp2[j] = (float*) malloc(sizeof(float) *w);
		}
		

		switch (i){
		case 0:
			animating = temp1;
			break;
		case 1:
			found = temp1;
			break;

		case 2: 
			temp = temp1;
			break;

		case 3:
			rotation_y = temp2;
			break;
		case 4: 
			rotation_progress = temp2;
			break;
		default:;
		}
	}
}

void vDelete(){

	for(int i = 0; i < 4; i++){
	
		int**temp;
		float**temp2;


	switch (i){
		case 0:
			temp = animating ;
			break;
		case 1:
			temp= found ;
			break;
		case 2:
			temp2= rotation_y ;
			break;
		case 3: 
			temp2 = rotation_progress;
			break;
		default:;
		}


		int h = memObj->getHeight();
		int w = memObj->getWidth();
				
		if(i<2){
	

		
			for(int j = 0; i < 4; j++)
				 free(temp[j]);


			free(temp);

		}else {
			for(int j = 0; i < 4; j++)
				free(temp2[j]);


			free(temp2);

		}
	}
}


void class_openGL::resize(int width, int height)
{
	// prevent division by zero
	if (height == 0) { height=1; }

	glViewport(0, 0, width, height);  

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width/(float)height, 0.01f, 10.0f);
	glMatrixMode(GL_MODELVIEW);
}


void class_openGL::keyPressed(unsigned char key, int x, int y) 
{
	// usleep(100);
	switch (key) {
	case 27:
		glutDestroyWindow(window); 
		exit(0);                   
		break;
	case 'a':
		for(int i = 0; i < memObj->getHeight();++i){
			for(int j=0; j < memObj->getWidth();j++){
				animating[i][j] = 1;

			}
		}

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


void class_openGL::reportGLError(const char * msg)
{
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		fprintf(stderr, "OpenGL Error: %s %s\n", msg, errString);
	}
	return;
}


void class_openGL::drawCard(int background){

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, cardfront[background]);

	glBegin(GL_QUADS);
	//glColor4f(1.0,1.0,1.0,0);

	float cardsize = 0.7;

	// front face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-cardsize, -cardsize,  0.01f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( cardsize, -cardsize,  0.01f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( cardsize,  cardsize,  0.01f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-cardsize,  cardsize,  0.01f);

	glEnd();

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, cardfront[background]);

	glBegin(GL_QUADS);

	// back face

	glTexCoord2f(1.0f, 0.0f); glVertex3f(-cardsize, -cardsize, -0.00f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-cardsize,  cardsize, -0.00f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( cardsize,  cardsize, -0.00f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( cardsize, -cardsize, -0.00f);

	glEnd();

	glDisable(GL_TEXTURE_2D);



}


void class_openGL::init(int width, int height)  
{
	FILE* file;
	GLsizei w, h;
	tgaInfo *info = 0;
	tgaInfo *info_back = 0;
	int mode;

	GLint format;
	char pfad[256]; //hier wird der Programmpfad gespeichert
	rotation_x = rotation_z = 0.0;

	for(int i = 0; i < memObj->getHeight(); i++)
		for(int j = 0; j<memObj->getWidth(); j++)
			rotation_y[i][j] = animating[i][j] = rotation_progress[i][j] =found[i][j] = temp[i][j]= 0;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);        
	glDepthFunc(GL_LESS);      
	glEnable(GL_DEPTH_TEST);  
	glShadeModel(GL_SMOOTH);  

	resize(width, height);


	info = tgaLoad("Cardback.tga");

	if (info->status != TGA_OK) {
		fprintf(stderr, "error loading texture image: %d\n", info->status);

		return;
	}

	if (info->width != info->height) {
		fprintf(stderr, "Image size %d x %d is not rectangular, giving up.\n",
			info->width, info->height);
		return;
	}

	mode = info->pixelDepth / 8;  // will be 3 for rgb, 4 for rgba
	glGenTextures(1, &cardback);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, cardback);
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

	for(int k = 0; k < 8; k++){
	
	//char* text = "cardfront" + k + ".tga";

	std::string str;
	str.append("cardfront");
	str.append(std::to_string(k+1));
	str.append(".tga");
	std::cout << str << std::endl;

	char*  str2 = (char*) str.c_str();

	info_back = tgaLoad(str2);

	if (info_back->status != TGA_OK) {
		fprintf(stderr, "error loading texture image: %d\n", info_back->status);

		return;
	}

	printf("CardBack: width: %d, height: %d\n, CardFront: width: %d, height: %d\n", info->width, info->height, info_back->width, info_back->height);


	mode = info_back->pixelDepth / 8;  // will be 3 for rgb, 4 for rgba
	glGenTextures(1, &cardfront[k]);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, cardfront[k]);
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
}




void class_openGL::display()
{
	GLfloat light_ambient[] = { q, w, e , r};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glLoadIdentity();  



	glTranslatef(0, 0, -9);

	glPushMatrix();

	glTranslatef(0,0,-1);
	glColor4f(0.9,1.0,0.8,0.9);

	glBegin(GL_QUADS);
	// front face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, -20.0f,  0.01f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 20.0f, -20.0f,  0.01f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 20.0f,  20.00f,  0.01f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f,  20.0f,  0.01f);

	glEnd();
	glPopMatrix();
	// glRotatef(rotation_x, 1, 0, 0);
	// glRotatef(rotation_y, 0, 1, 0);
	//  glRotatef(rotation_z, 0, 0, 1);

	glLightfv(GL_LIGHT0, GL_SPECULAR, light_ambient);
	glEnable(GL_LIGHT0);


	for(int i = 0; i < memObj->getHeight(); i++){
		for(int j = 0; j<memObj->getWidth(); j++){

			glPushMatrix();

			glTranslatef(coordsHARD[j], coordsHARD[i], 0);


			glRotatef(rotation_y[i][j], 0, 1, 0);
			if(found[i][j]==0)
				drawCard(memObj->getCardType(i,j));

			/*
			glEnable (GL_BLEND);
			glBlendFunc (GL_ONE, GL_SRC_COLOR);
			*/


			glPopMatrix();

			if (animating[i][j]) {
				if(rotation_progress[i][j] == 180)
					temp[i][j] = (temp[i][j])?0:1;
				//rotation_x += 0.2f;
				if(rotation_progress[i][j] > 180){
					printf("ERROR, ROTATION: %d \n", rotation_progress[i][j]);
					if(rotation_y[i] [j]>90 || rotation_y[i][j]<270)
						rotation_y[i][j] = 180;
					else
						rotation_y[i][j] = 0;

				}

				if(rotation_progress[i][j] >= 180){
					rotation_progress[i][j] = animating[i][j] = 0;
				}else{
					rotation_y[i][j] += rotating_speed;
					rotation_progress[i][j] += rotating_speed;

				}
			}

			//rotation_z += 0.1f;
			//glutPostRedisplay();
		}

	}
	if((temp[0][0] + temp[1][0] )==2)
		found[0][0]=found[1][0]=1;

	glutSwapBuffers();
}



void class_openGL::timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(15, class_openGL::timer, 1);
}

void class_openGL::mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:    /* spin scene around */
		if (state == GLUT_DOWN){

			if(x>75 && x<265 && y > 145 && y < 335){
				mousePress = 2;

			}
			printf("mouse x = %d, mouse y = %d\n", x, y);
			if(x>370 && x<560 && y > 145 && y < 335){
				mousePress = 1;

			}

		} else if (state == GLUT_UP){
			//printf("Mouse Press = %d\n",mousePress);
			if(mousePress!=0)
				animating[mousePress-1][0] = 1;
			mousePress=0;



		}
	}

	glutPostRedisplay();
}

void class_openGL::start(int argc, char **argv){

	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
	glutInitWindowSize(800, 600);  
	glutInitWindowPosition(0, 0);  
	window = glutCreateWindow("Memory");  
	glutDisplayFunc(&display);  
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&keyPressed);

	init(800, 600);
	glutMouseFunc(mouse);
	glutTimerFunc(15, timer, 1);
	glutMainLoop();  

}