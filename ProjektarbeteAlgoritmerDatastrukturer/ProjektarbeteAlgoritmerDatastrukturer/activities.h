#pragma once
#include "theData.h"
#include "AverageDay.h"
#include "AverageAll.h"
#include <vector>

class activities
{
private:
	// två vektorer, en för inne och en för ute. Vektorer av datatypen "theData", pekarvektorer.
	std::vector<theData *> outside;
	std::vector<theData *> inside;

	// vector till alla medelvärden
	std::vector<AverageAll *> Average; 

	// variabler
	std::string answer, searchDate;
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
	void printMould(); // printar ut mögelrisk.


};

