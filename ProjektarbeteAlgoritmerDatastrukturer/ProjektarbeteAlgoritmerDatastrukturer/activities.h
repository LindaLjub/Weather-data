#pragma once
#include "theData.h"
#include "AverageDay.h"

class activities
{
private:
	// tv� vektorer, en f�r inne och en f�r ute. Vektorer av datatypen "theData", pekarvektorer.
	std::vector<theData *> outside;
	std::vector<theData *> inside;

	//pekarvectorer av typen "AverageDay".
	std::vector<AverageDay *> AverageOutside;
	std::vector<AverageDay *> AverageInside;

	// variabler
	std::string answer, searchDate;
	float summa = 0, summa2 = 0;
	int counter = 0, counter2 = 0;
	int sizeVectorOutside, sizeVectorInside, sizeVectorAverageOut, sizeVectorAverageIn;

public:
	activities();
	~activities();

	// metoder till klassen.
	void indata();			// Metod som h�mtar in data fr�n fil.

	void menuOne();			// Huvudmenyn.
	void menuInside();		// Undermeny inomhus
	void menuOutside();		// Undermeny utomhus
	void printAverage(std::string); // Printar medeltemp alla dagar.
	double searchDay(std::string, std::string, std::string); // Hittar r�tt day, returnerar medeltemp/medelfuktighet.

	void metrologisk(); // Hittar dagen f�r metrologisk h�st och vinter.
	void mould();
	void printMould();


	// Nytt test
	void testAverage();
	void testCount(std::string, float &, float &, std::string);


};

