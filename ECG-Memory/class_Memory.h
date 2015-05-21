#pragma once
#ifndef CLASS_MEMORY_PROJECT
#define CLASS_MEMORY_PROJECT


#include <stdlib.h> // for exit
//#include <unistd.h> // for usleep
#include <stdio.h>
#include <time.h.>
#include <signal.h>


enum difficulty {EASY, MEDIUM, HARD};

class Memory{


	difficulty current_div;
	bool ongoing;		//Solage das spiel nicht beendet wird, auf true gesetzt
	int field_width;	//breite des Spielfelds
	int field_height;	//höhe des Spielfelds
	int ***field;    // x, y, info field[x][y][info] || info == 2 || info[0] = cardname info[1] = state (0 not shown, 1 shown, 2 disabled), 
	char*  cardnames[8];	//alle kartennamen
	char* cardback;			//kartenrücken
	int MAXCARDS;			//speichert die gesamtzahl der Karten
	int pairsfound;			//bereits gefundene Paare
	int coords[2];



	/* Utility Functions */

	void startNewGame(difficulty dif); //initialisert das spiel
	void initialize_field();	//initialisiert das Feld
	void deleteData(); //gibt alles frei
	void printField(); //gibt das Feld aus
	
	

	/*Card Functions */

	void getCoordinates (int cardNo);  // wandelt die Kartennummer in die Koordinaten um
	int turn_card(int cardNo); //dreht eine karte um
	void setState(int cardNo, int state); //ändert den State einer karte
	int getValue(int cardNo);  // gibt den Value einer karte zurück
	void checkCorrect(int values[2]); //vergleicht 2 karten, es werden die Kartennummern übergeben


	
	


public:
	Memory(difficulty dif);
	difficulty getCurrentDif(){return current_div;};
	int getHeight(){return field_height;};
	int getWidth(){return field_width;};
	int getMaxCards(){return MAXCARDS;};
	int getCardType(int x, int y){return field[x][y][0];};
	~Memory();
	
	
	
	void play();
	

};



#endif