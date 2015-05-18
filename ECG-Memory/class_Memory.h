#include <stdlib.h> // for exit
//#include <unistd.h> // for usleep
#include <stdio.h>


class Memory{

	bool ongoing;
	int field_width;
	int field_height;
	int ***field;    // x, y, info field[x][y][info] info[0] = cardname info[1] = state (0 not shown, 1 shown, 2 disabled), 
	char*  cardnames[8];
	char* cardback;
	int MAXCARDS;
	int pairsfound;

	int superRandomValue;
	int superRandom2;
	int coords[2];

	int showCard(int value);
	void getCoordinates (int cardNo);
	void setup_8game();
	void checkCorrect(int values[2]);
	void initialize_field();
	int turn_card(int cardNo);
	void setState(int cardNo, int state);
	int getValue(int cardNo);
	

public:
	Memory(int pairs){startNewGame(pairs);ongoing = true;};
	void startNewGame(int pairs);
	int clickCard(int value);
	void printField();
	
	void make_turn();
	
	bool getOngoing(){return ongoing;};

};