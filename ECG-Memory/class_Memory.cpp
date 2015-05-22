#include "class_Memory.h"

int debug =1;


	/* Utility Functions */


void Memory::startNewGame(difficulty dif){

	
	pairsfound = 0;
	current_div = dif;
//	dif = HARD;

	//printf("DIF: %d, %s\n",dif,dif);
	
	switch(dif){

	case 0: field_height = 2;
			field_width = 3;
			break;

	case 1: field_height = 3;
			field_width = 4;
			break;

	case 2: field_height = 4;
			field_width = 4 ;
			break;

	default: printf("ERROR");
		break;
	}


		
	MAXCARDS = (field_height * field_width) -1;
	initialize_field();
	
	//int current_card_pair = 0;
	int cards_set = 0;
	int pointed_card = 0;

	/*for(int i = 0; i < field_height; i++){
		printf(" | ");
		for(int j = 0; j < field_width; j++)
			printf("%2d | ", field[i][j][0]);
		
		printf("\n");
	}*/

	int randomNummer;
	unsigned int seed = (unsigned int)time(NULL);
	int breakpoint = 0;
	for(int i = 0; i < ((MAXCARDS+1)/2); i++){
		cards_set = 0;
		breakpoint = 0;
		while(cards_set<2){
			++breakpoint;
			
			srand(seed);
			randomNummer = rand();
			if(cards_set == 0)
				;//printf("Random Nummer: %d || Seed: %d \n", randomNummer, seed);
			else
				++randomNummer;
			seed = (unsigned int) randomNummer;
			pointed_card = (((pointed_card)+ rand()) % (field_height * field_width)) ;
			
			getCoordinates(pointed_card);
	
			int* punkt = &field[coords[0]][coords[1]][0];
			if(*punkt == -1){
				cards_set++;
				*punkt = i;
				
			}else if(breakpoint>20){
				while(breakpoint != 0){
					

					pointed_card = ++pointed_card % (field_height * field_width);
			
					getCoordinates(pointed_card);
					//printf("X: %d Y: %d",coords[0],coords[1]);
					int* punkt = &field[coords[0]][coords[1]][0];
					if(*punkt == -1){
						*punkt = i;
						cards_set++;
						breakpoint = 0;
					}
			
				}

			}
		}
	}

//	printField();
}

void Memory::initialize_field(){

	deleteData();

	int *** temp_field = (int***) malloc(sizeof(int**) * field_height);

	for(int i = 0; i < field_height; i++)
		temp_field[i] = (int**) malloc(sizeof(int*) * field_width);


	for(int i = 0; i < field_height; i++)
		for(int j = 0; j < field_width; j++){
			temp_field[i][j] = (int*) malloc(sizeof(int) * 2);
			temp_field[i][j][0] = -1;
			temp_field[i][j][1] = debug;
			

		}
	field = temp_field;

}

void Memory::deleteData(){

	if(field == NULL)
		return;


	int*** temp_field = field;
	field = NULL;
	
	for(int i = 0; i < field_height; i++)
		for(int j = 0; j < field_width; j++)
			if(temp_field[i][j] !=NULL)
				free(temp_field[i][j]);

		for(int i = 0; i < field_height; i++)
			if(temp_field[i]!=NULL)
				free(temp_field[i]);

     if(temp_field != NULL)	
		 free(temp_field);


}


void Memory::printField(){

	char* content;
	printf("\n FIELD: \n");

	for(int i = 0; i < field_height; i++){
		printf(" | ");
		for(int j = 0; j < field_width; j++){
			switch (field[i][j][1])
			{
			case 0: content = cardback;
				break;
			case 1: content = cardnames[field[i][j][0]];
				break;
			case 2: content = "";
				break;
			default: content = "ERROR";
				break;
			
			}
			printf("(%2d) %10s | ",(field_width*i)+j, content);
		}
		printf("\n");
	}

}



/*Card Functions */

void Memory::getCoordinates (int cardNo){
	coords[0] = cardNo / field_width;
	coords[1] = cardNo % field_width;

}

int Memory::turn_card(int value){

	getCoordinates(value);
	int* state_pointer = &field[coords[0]][coords[1]][1];
	//printf("value of statepointer: %d\n" , *state_pointer);

	if(*state_pointer == 1)
		*state_pointer = 0;
	else if(*state_pointer == 0)
		*state_pointer = 1;
	else
		return 1;

		//printf("value of statepointer: %d\n" , *state_pointer);
		//printf("value of field %d\n",field[coords[0]][coords[1]][1]);
	return 0;


}


void Memory::setState(int cardNo, int state){
		getCoordinates(cardNo);
		field[coords[0]][coords[1]][1] = state;
}

int Memory::getValue(int cardNo){
	getCoordinates(cardNo);
	return field[coords[0]][coords[1]][0];
}

void Memory::checkCorrect(int values[2]){
	

	if(getValue(values[0]) == getValue(values[1])){
		for(int i=0;i<2; i++)
			setState(values[i],2);		
		pairsfound++;
		
	}else{
		for(int i=0;i<2; i++)
			turn_card(values[i]);
	}

	//printField();

}





/* Public functions */

Memory::Memory(difficulty dif){

char* tempNames[8]	= {"street", "city", "hill", "play", "window", "sunrise", "camp", "hotel"};
cardback = "#########";
ongoing = true;
field = NULL;
printf("Setting up Memory..\n");

	for(int i = 0; i < 8; i++)
		cardnames[i] = tempNames[i];


startNewGame(dif);
}

Memory::~Memory(){

	deleteData();

}

void Memory::play(){

	int value[2];
	int turn_return;
	int turns = 0;

	//printField();

	while(ongoing){
		printf("Enter Value: \n");
		scanf("%d",&value[turns]);

		if(value[turns]>20)
			raise(SIGTERM);

		if(value[turns]<0||value[turns]>15)
			continue;

	
	
		turn_return = turn_card(value[turns]);
		if(turn_return){
			printf("Error, no card @ %d\n", value[turns]);
			continue;
		}

		//printField();
		turns++;

		if(turns>1){
			checkCorrect(value);
			turns = 0;
		}
		if(pairsfound == (MAXCARDS +1) /2)
			ongoing = false;
	}
}
	

