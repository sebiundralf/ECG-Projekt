#include <stdlib.h> // for exit
//#include <unistd.h> // for usleep
#include <stdio.h>
#include "class_Memory.h"


int main(int argc, char **argv){

	Memory mem(8);
	printf("starting..\n");
	mem.printField();

	while(mem.getOngoing()){
/*		int value = 0;
		printf("Enter Value: \n");
		scanf("%d",&value);
		if(value>20)
			return 0;
		if(value<0||value>15)
			continue;
	
	//	mem.printField();*/
			mem.make_turn();


	}




}