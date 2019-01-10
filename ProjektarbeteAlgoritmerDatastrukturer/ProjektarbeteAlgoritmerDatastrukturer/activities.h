#pragma once
#include "theData.h"
#include "AverageDay.h"
#include "AverageAll.h"
#include <vector>

class activities
{
private:
	// tv� vektorer, en f�r inne och en f�r ute. Vektorer av datatypen "theData", pekarvektorer.
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

	void indata();			// Metod som h�mtar in data fr�n fil.
	void menuOne();			// Huvudmenyn.
	void menuInside();		// Undermeny inomhus
	void menuOutside();		// Undermeny utomhus

	void printAverage(std::string); // Printar medeltemp alla dagar.
	double searchDay(std::string, std::string, std::string); // Hittar r�tt day, returnerar medeltemp.
	void testAverage();	// hittar unika dagar.
	void testCount(std::string, float &, float &, std::string); //R�knar ut medelv�rde.

	void metrologisk(); // Hittar dagen f�r metrologisk h�st och vinter.
	void mould();		// M�gelrisk.
	void printMould(); // printar ut m�gelrisk.





};

