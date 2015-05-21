#include <stdlib.h> // for exit
//#include <unistd.h> // for usleep
#include <stdio.h>
#include "class_Memory.h"

int main1(int argc, char **argv);

int main(int argc, char **argv){

	bool memory = false;
	bool graphic = true;

	if(graphic)
		main1(argc, argv);
	
	if(memory){
	Memory mem(MEDIUM);
	printf("starting..\n");
	mem.play();
	}

}




