#include <stdlib.h> // for exit
//#include <unistd.h> // for usleep
#include <stdio.h>
#include "class_Memory.h"

int main1(int argc, char **argv);

int main(int argc, char **argv){

	bool memory = true;
	bool graphic = false;

	if(graphic)
		main1(argc, argv);
	
	if(memory){
	Memory mem(EASY);
	printf("starting..\n");
	mem.play();
	}

}




