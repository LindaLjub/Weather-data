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

	//pekarvectorer av typen "AverageDay".
	std::vector<AverageDay *> AverageOutside;
	std::vector<AverageDay *> AverageInside;

	std::vector<AverageAll *> Average; // testar med bool

	// variabler
	std::string answer, searchDate;
	int sizeVectorOutside, sizeVectorInside, sizeVectorAverageOut, sizeVectorAverageIn;

public:
	activities();
	~activities();

	void indata();			// Metod som hämtar in data från fil.
	void menuOne();			// Huvudmenyn.
	void menuInside();		// Undermeny inomhus
	void menuOutside();		// Undermeny utomhus

	void printAverage(std::string); // Printar medeltemp alla dagar.
	double searchDay(std::string, std::string, std::string); // Hittar rätt day, returnerar medeltemp.
	void testAverage();	// hittar unika dagar.
	void testCount(std::string, float &, float &, std::string); //Räknar ut medelvärde.

	void metrologisk(); // Hittar dagen för metrologisk höst och vinter.
	void mould();		// Mögelrisk.
	void printMould(); // printar ut mögelrisk.





};

