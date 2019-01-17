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
	// två vektorer, en för inne och en för ute. Vektorer av datatypen "theData", pekarvektorer.
	std::vector<theData *> outside;
	std::vector<theData *> inside;

	// vector till alla medelvärden
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

	void indata();			// Metod som hämtar in data från fil.
	void menuOne();			// Huvudmenyn.
	void menuInside();		// Undermeny inomhus
	void menuOutside();		// Undermeny utomhus

	void average(); // Räknar ut medelvärde.
	void printAverage(std::string); // Printar medeltemp alla dagar.
	void searchDay(std::string, std::string); // Hittar rätt day, returnerar medeltemp.
	void metrologisk(); // Hittar dagen för metrologisk höst och vinter.
	void mould();		// Mögelrisk.
	void printMould(std::string); // printar ut mögelrisk.

	void diff(); // räknar ut differensen mellan inne/ute temp varje dag.
	void printdiff(); // printar ut störst/minst diff

	void doorOpen();
	void hoursOpen();
	void printDoor();


};

