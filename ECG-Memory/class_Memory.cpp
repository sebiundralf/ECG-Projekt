#include "class_Memory.h"


void Memory::startNewGame(int pairs){

	char* tempNames[8]	= {"Hund", "Katze", "Pferd", "Blume", "Haus", "Computer", "Mann", "Frau"};
	superRandomValue = 7;
	superRandom2 = 3;
	pairsfound = 0;
	for(int i = 0; i < 8; i++)
		cardnames[i] = tempNames[i];
	cardback = "#########";
	pairs = 8;

	switch(pairs){

	case 8: setup_8game();
			break;
	default: printf("ERROR");
		break;
	}


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

void Memory::setup_8game(){

	field_height = field_width = 4;
	MAXCARDS = 15;
	initialize_field();
	
	//int current_card_pair = 0;
	int cards_set = 0;
	int pointed_card = 0;

	for(int i = 0; i < field_height; i++){
		printf(" | ");
		for(int j = 0; j < field_width; j++){
			printf("%2d | ", field[i][j][0]);

		}
		printf("\n");
	}


	for(int i = 0; i < 8; i++){
		cards_set = 0;
		while(cards_set<2){
			pointed_card = ((pointed_card)+ rand()) % (field_height * field_width);
			getCoordinates(pointed_card);

			int* punkt = &field[coords[0]][coords[1]][0];
			if(*punkt == -1){
				*punkt = i;
				cards_set++;
			}

		}




		



	}



}

void Memory::getCoordinates (int cardNo){
	coords[0] = cardNo / field_height;
	coords[1] = cardNo % field_width;

}

void Memory::initialize_field(){


	int *** temp_field = (int***) malloc(sizeof(int**) * field_height);

	for(int i = 0; i < field_height; i++)
		temp_field[i] = (int**) malloc(sizeof(int*) * field_width);


	for(int i = 0; i < field_height; i++)
		for(int j = 0; j < field_width; j++){
			temp_field[i][j] = (int*) malloc(sizeof(int) * 2);
			temp_field[i][j][0] = -1;
			temp_field[i][j][1] = 0;
			

		}
	field = temp_field;

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

void Memory::make_turn(){

	int value[2];
	int turn_return;
	int turns = 0;
	while(ongoing){
		printf("Enter Value: \n");
		scanf("%d",&value[turns]);
		/*if(value>20)
			return;*/
		if(value[turns]<0||value[turns]>15)
			continue;
	
		turn_return = turn_card(value[turns]);
		if(turn_return){
			printf("Error, no card @ %d\n", value[turns]);
			continue;
		}

		printField();
		turns++;

		if(turns>1){
			checkCorrect(value);
			turns = 0;
		}
		if(pairsfound == (MAXCARDS +1) /2)
			ongoing = false;
	}
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

	printField();

}