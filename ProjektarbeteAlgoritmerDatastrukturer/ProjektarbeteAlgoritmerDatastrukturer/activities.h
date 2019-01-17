#pragma once
#include "theData.h"
#include "AverageDay.h"
#include "AverageAll.h"
#include <vector>
#include <algorithm>
#include <iostream>



class activities
{
private:
	// tv� vektorer, en f�r inne och en f�r ute. Vektorer av datatypen "theData", pekarvektorer.
	std::vector<theData *> outside;
	std::vector<theData *> inside;

	// vector till alla medelv�rden
	std::vector<AverageAll *> Average; 

	std::vector<theData *> doorIn;
	std::vector<theData *> doorOut;
	std::vector<theData *> open;

	// variabler
	std::string answer, searchDate;
	std::string metro = " Not found..";
	std::string metroW = " Not found..";

	int sizeVectorOutside, sizeVectorInside, checkDate = 0;

public:
	activities();
	~activities();

	void indata();			// Metod som h�mtar in data fr�n fil.
	void menuOne();			// Huvudmenyn.
	void menuInside();		// Undermeny inomhus
	void menuOutside();		// Undermeny utomhus

	void average(); // R�knar ut medelv�rde.
	void printAverage(std::string); // Printar medeltemp alla dagar.
	void searchDay(std::string, std::string); // Hittar r�tt day, returnerar medeltemp.
	void metrologisk(); // Hittar dagen f�r metrologisk h�st och vinter.
	void mould();		// M�gelrisk.
	void printMould(std::string); // printar ut m�gelrisk.

	void diff(); // r�knar ut differensen mellan inne/ute temp varje dag.
	void printdiff(); // printar ut st�rst/minst diff

	void doorOpen();
	void hoursOpen();
	void printDoor();


};

