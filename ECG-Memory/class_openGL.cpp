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

static int selectedCorrect=0;
static int currentAnimating = 0;
static int turn = 0;
static int selectedCards[2] = {0,0};
static int rotating_speed;
static int q;
static int w;
static int e ;
static int r;
static int mousex;
static int mousey;
static int mousePress;
static int mousePressX;
static int mousePressY;
static float clickMaskX[4][2];
static float clickMaskY[4][2];
static int testv;
static int animation = 0;
static bool light_enabled = true;
static int hover[4][4] = {{0,0,0,0},
							{0,0,0,0},
							{0,0,0,0},
							{0,0,0,0}};

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 5.0 };
GLfloat light_position[3][4] = {  { 0.0, 0.0, 1.0, 0.0 },
								{ 1.0, 0.0, 1.0, 0.0 },
							    { 0.0, 1.0, 1.0, 0.0 }  };

GLfloat light_emission[] = {0.6,0.6,0.6,0.0};
GLfloat light_emission2[] = {0.4,0.4,1.0,1.0};
GLfloat light_emission3[] = {0.6,0.6,1.0,1.0};

  GLfloat light_diffuse[] = { 8.0, 8.0,8.0, 1.0 };
  GLfloat light_specular[] = { 8.0, 8.0, 8.0, 1.0 };
  GLfloat light_ambient[] = { 7.0,7.0,7.0, 1.0 };


float factor (0.8);

static float ***coords;
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
	mousePressX = 0;
	mousePressY = 0;
	mousePress = 0;
	memObj = mem;
	dif = mem->getCurrentDif();
	vInitialize();
}

void class_openGL::vInitialize(){

	
	int h = memObj->getHeight();
		int w = memObj->getWidth();

	for(int i = 0; i < 5; i++){

		
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
		float step = 1.5;
		float tempx;
		float tempy;

	coords = (float***) malloc(sizeof(float**) * h);

	for(int i = 0; i < h; i++)
		coords[i] = (float**) malloc(sizeof(float*)* w);

	for(int i = 0; i < h; i++){
		for(int j = 0; j < w; j++){
			coords[i][j] = (float*) malloc(sizeof(float)* 2);
			tempx = coords[i][j][0] = ((j) * step) -  ((step * (w-1)) /2 );
			tempy = coords[i][j][1] = (((i) * step) -  ((step * (h-1)) /2 )) * -1;
		
			//printf("co %d/%d: %2.2f/%2.2f | ", i, j, tempx, tempy);
		}
	//	printf("\n"); 
	}

	switch(dif)
	{
	case EASY: 

	case MEDIUM:

	case HARD:
		{float tempMask[4][2] =  {{85,232},
							{246,393},
							{407,554},
							{568,715}};
		
		for(int i = 0; i < 4; i++){
			clickMaskX[i][0] = clickMaskY[i][0] = tempMask[i][0];
			clickMaskX[i][1] = clickMaskY[i][1] = tempMask[i][1];
		}
		}
	//	printf("clickMask initialisiert\n");
		break;


	default:
		printf("NOPE\n");
		break;
	}
}

void vDelete(){
		int h = memObj->getHeight();
		int w = memObj->getWidth();

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


				
		if(i<2){
			for(int j = 0; i < h; j++)
				 free(temp[j]);
			free(temp);

		}else {
			for(int j = 0; i < h; j++)
				free(temp2[j]);
			free(temp2);
		}
	}

	for(int i = 0; i < h; i++){
		for(int j = 0; j < w; j++)
			free(coords[i][j]);
		free(coords[i]);
	}
	
	free(coords);
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
				if(animating[i][j] != 1){
					animating[i][j] = 1;
					currentAnimating++;
				};
			
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
		rotating_speed = 1;
		break;
	case '5':
		rotating_speed = 5;
		break;
	case '0':
		rotating_speed = 15;
		break;
	case 'f':
		printf("Window size: %d x %d\n", glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		
		for(int i = 0; i < memObj->getHeight(); i++)
			for(int j = 0; j < memObj->getWidth(); j++)
			//	printf("y=%d x= %d anim= %2.2f\n",i,j,rotation_y[i][j]);
		glutPostRedisplay();
	case 'q':
		memObj->printStates();
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
	case 'l':
		light_enabled = !light_enabled;
		if(light_enabled)
			  glEnable(GL_LIGHTING);
		else
			  glDisable(GL_LIGHTING);
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


void class_openGL::drawBorder(float cardsize){

	
		/* if found */

	

	float thickness = 0.04f;

	glPushMatrix();

	


	glBegin(GL_QUADS);


	// back face

	glTexCoord2f(1.0f, 0.0f); glVertex3f(-(cardsize+(thickness)), -(cardsize+(thickness)), -0.00f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-(cardsize+(thickness)),  (cardsize+(thickness)), -0.00f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( (cardsize+(thickness)),  (cardsize+(thickness)), -0.00f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( (cardsize+(thickness)), -(cardsize+(thickness)), -0.00f);

	glEnd();

	glColor3f(1.0f,1.0f,01.0f);
		
	glPopMatrix();

}

void class_openGL::drawCard(int background, int x, int y){
		float cardsize = 0.7;
		bool vdrawBorder = false;


	if(hover[y][x]==1 && rotation_y[y][x]==0 &&  turn<2){
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_emission2);
		glColor3f(1.0f,0.0f,0.0f);
		vdrawBorder = true;
	}
	else{
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_emission);
	}

	
	if(temp[y][x]==1){
//	 glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_emission3);
	 		glColor3f(0.0f,1.0f,0.0f);
		vdrawBorder = true;

	}

	if(hover[y][x]==1)
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_emission2);


	if(vdrawBorder)
		drawBorder(cardsize);

	

	glPushMatrix();



	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, cardback);


		glTranslatef(0, 0, 0.01f);

	glBegin(GL_QUADS);
	//glColor4f(1.0,1.0,1.0,0);



	// front face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-cardsize, -cardsize,  0.00f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( cardsize, -cardsize,  0.00f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( cardsize,  cardsize,  0.00f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-cardsize,  cardsize,  0.00f);

	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();


	glPushMatrix();
		glTranslatef(0, 0, -0.01f);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, cardfront[background]);


		glRotatef(180, 0, 1, 0);
	glBegin(GL_QUADS);

	// back face

	glTexCoord2f(1.0f, 0.0f); glVertex3f(-cardsize, -cardsize, -0.00f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-cardsize,  cardsize, -0.00f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( cardsize,  cardsize, -0.00f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( cardsize, -cardsize, -0.00f);

	glEnd();

	glDisable(GL_TEXTURE_2D);

	


	glPopMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_emission);




		

	
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

	  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_emission);

  for(int i = 1; i < 3; i++){

  //glLightfv(GL_LIGHT0+i, GL_DIFFUSE, light_diffuse);
 // glLightfv(GL_LIGHT0+i, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0+i, GL_POSITION, light_position[i]);
    glLightfv(GL_LIGHT0+i, GL_AMBIENT, light_ambient);

	  glEnable(GL_LIGHT0+i);
  }

  
  glEnable(GL_LIGHTING);

  glEnable(GL_DEPTH_TEST);



	for(int i = 0; i < memObj->getHeight(); i++)
		for(int j = 0; j<memObj->getWidth(); j++)
			rotation_y[i][j] = animating[i][j] = rotation_progress[i][j] =found[i][j] = temp[i][j]= 0;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);        
	glDepthFunc(GL_LESS);      
	glEnable(GL_DEPTH_TEST);  
	glShadeModel(GL_SMOOTH);  

	resize(width, height);


	info = tgaLoad("textures/Cardback.tga");

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
	str.append("textures/cardfront");
	str.append(std::to_string(k+1));
	str.append(".tga");
	//std::cout << str << std::endl;

	char*  str2 = (char*) str.c_str();

	info_back = tgaLoad(str2);

	if (info_back->status != TGA_OK) {
		fprintf(stderr, "error loading texture image: %d\n", info_back->status);

		return;
	}

	//printf("CardBack: width: %d, height: %d\n, CardFront: width: %d, height: %d\n", info->width, info->height, info_back->width, info_back->height);


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
	
	if(currentAnimating==0 && turn>=2){


	if(turn==2){

		#ifdef _WIN32
		Sleep(100);
		#endif
				selectedCorrect = memObj->checkCorrect(selectedCards);
				++turn;
				for(int i = 0; i < 2; ++i){
				memObj->getCoordinates(selectedCards[i]);
				animating[memObj->coords[0]] [memObj->coords[1]] = 1 ;
				//memObj->turn_card(selectedCards[i]);
				++currentAnimating;
				if(selectedCorrect ==1)
					temp[memObj->coords[0]] [memObj->coords[1]] = 1 ;	

				printf("Returned: X %d, Y%d, State: %d\n",memObj->coords[0],memObj->coords[1],memObj->getState(selectedCards[i]));
				}
					printf("After rereturn\n");
				memObj->printStates();
				//if(memObj->checkCorrect(selectedCards));
			}else{
				if(selectedCorrect ==1)
				{
					selectedCorrect = 0;
					for(int i = 0; i < 2; ++i){
						memObj->getCoordinates(selectedCards[i]);
						found[memObj->coords[0]] [memObj->coords[1]] = 1 ;	
					}
				}
				printf("After check\n");
				memObj->printStates();
				turn = 0;	
		}

	}
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
/*
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_ambient);
	glEnable(GL_LIGHT0);*/
	
		glColor3f(1.0f,1.0f,01.0f);
	float coordX;
	float coordY;

	for(int i = 0; i < memObj->getHeight(); i++){
		for(int j = 0; j<memObj->getWidth(); j++){
			
			if(rotation_y[i][j]>=360)
				rotation_y[i][j] = rotation_y[i][j] - 360;

			glPushMatrix();
			coordX = coords[i][j][0];
			coordY = coords[i][j][1];
			glTranslatef(coordX,coordY , 0);


			glRotatef(rotation_y[i][j], 0, 1, 0);

		/*	if(i==1 && j==3)
				glRotatef(animation+=5,0,1,0);*/

			if(found[i][j]==0)
				drawCard(memObj->getCardType(i,j),j,i);

			/*
			glEnable (GL_BLEND);
			glBlendFunc (GL_ONE, GL_SRC_COLOR);
			*/


			glPopMatrix();

			

			if (animating[i][j]) {
				
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
					//temp[i][j] = (temp[i][j])?0:1;
					currentAnimating--;
						
				}else{
					rotation_y[i][j] += rotating_speed;
					rotation_progress[i][j] += rotating_speed;

				}
			}

			//rotation_z += 0.1f;
			//glutPostRedisplay();
		}

	}
/*	if((temp[0][0] + temp[1][0] )==2)
		found[0][0]=found[1][0]=1;
*/
	glutSwapBuffers();
}



void class_openGL::timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(15, class_openGL::timer, 1);
}

void class_openGL::mouseMotion(int x, int y){
	
	float x1;
	float x2;
	float y1;
	float y2;


	for(int i = 0; i < memObj->getHeight(); ++i){
				for(int j = 0; j < memObj->getWidth(); ++j){
					++testv;
					x1 = (clickMaskX[j][0] * glutGet(GLUT_WINDOW_HEIGHT)) / 800 + (glutGet(GLUT_WINDOW_WIDTH)-glutGet(GLUT_WINDOW_HEIGHT))/2;
					x2 = (clickMaskX[j][1] * glutGet(GLUT_WINDOW_HEIGHT)) / 800 +  (glutGet(GLUT_WINDOW_WIDTH)-glutGet(GLUT_WINDOW_HEIGHT))/2;
					y1 = clickMaskY[i][0] * glutGet(GLUT_WINDOW_HEIGHT)/ 800;
					y2 = clickMaskY[i][1] * glutGet(GLUT_WINDOW_HEIGHT) / 800;
				

					if(x >= x1 &&  x<= x2 && y >= y1 && y <= y2  ) {
						hover[i][j] = 1;
						//printf("hover.");
					}
					else
						hover[i][j] = 0;

				}
	}

}

void class_openGL::mouse(int button, int state, int x, int y)
{
	float x1;
	float x2;
	float y1;
	float y2;

	if(turn<2)
	switch (button) {
	case GLUT_LEFT_BUTTON:    /* spin scene around */

		if (state == GLUT_DOWN){
			testv = 0;
			for(int i = 0; i < memObj->getHeight(); ++i){
				for(int j = 0; j < memObj->getWidth(); ++j){
					++testv;
					x1 = (clickMaskX[j][0] * glutGet(GLUT_WINDOW_HEIGHT)) / 800 + (glutGet(GLUT_WINDOW_WIDTH)-glutGet(GLUT_WINDOW_HEIGHT))/2;
					x2 = (clickMaskX[j][1] * glutGet(GLUT_WINDOW_HEIGHT)) / 800 +  (glutGet(GLUT_WINDOW_WIDTH)-glutGet(GLUT_WINDOW_HEIGHT))/2;
					y1 = clickMaskY[i][0] * glutGet(GLUT_WINDOW_HEIGHT)/ 800;
					y2 = clickMaskY[i][1] * glutGet(GLUT_WINDOW_HEIGHT) / 800;
				

					if(x >= x1 &&  x<= x2 && y >= y1 && y <= y2  ){
					/*	printf("Values: x = %2.1f - %2.1f, y = %2.1f, %2.1f\n", x1, x2, y1, y2);
						printf("x = %d, y = %d\n", j, i);
						printf("Calculation: cmx1 j %2.0f, cmx2 j %2.0f, cmy1 i %2.0f, cmy2 i %2.0f, wh %d, ww %d\n", clickMaskX[j][0], clickMaskX[j][1], clickMaskY[i][0], clickMaskY[i][1],  glutGet(GLUT_WINDOW_HEIGHT), glutGet(GLUT_WINDOW_WIDTH));
						*/
						

						if(memObj->getState(i,j)==0){
						mousePressX = j;
						mousePressY = i;
						mousePress = 1;
						printf("Mouse Down, go turning card\n");
						}

						printf("Mouse Down X: %d, Y: %d, State %d, turn: %d\n", j, i, memObj->getState(i,j), turn);
						break;
					}
					if(mousePress!=0)
						break;
				}
				if(mousePress!=0)
						break;

			}
			//printf("mouse x = %d, mouse y = %d\n", x, y);
			
			
			
		} else if (state == GLUT_UP){
			//printf("Mouse Press = %d\n",mousePress);
			if(mousePress!=0){
				animating[mousePressY][mousePressX] = 1;
				++turn;

			currentAnimating++;
			selectedCards[turn-1] = testv-1;
			memObj->turn_card(testv-1);
					printf("Mouse UP curr ani %d, turn %d, state %d\n", currentAnimating, turn,memObj->getState(testv-1));
			

			//memObj->printField();
			}
				mousePressX=mousePressY=mousePress=0;

		}
	}

	glutPostRedisplay();
}

void class_openGL::start(int argc, char **argv){

	printf("Starting openGL ..\n");
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
	glutInitWindowSize(800, 800);  
	glutInitWindowPosition(0, 0);  
	window = glutCreateWindow("Memory");  
	glutDisplayFunc(&display);  
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&keyPressed);

	init(800, 800);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseMotion);
	glutTimerFunc(15, timer, 1);
	//glutFullScreen();
	glutMainLoop();  

}