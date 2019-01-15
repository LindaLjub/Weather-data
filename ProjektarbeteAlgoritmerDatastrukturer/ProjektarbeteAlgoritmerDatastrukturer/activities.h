#pragma once
#include "theData.h"
#include "AverageDay.h"
#include "AverageAll.h"
#include <vector>
#include <algorithm>

class activities
{
private:
	// två vektorer, en för inne och en för ute. Vektorer av datatypen "theData", pekarvektorer.
	std::vector<theData *> outside;
	std::vector<theData *> inside;

	// vector till alla medelvärden
	std::vector<AverageAll *> Average; 

	std::vector<theData *> door;

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



	
	void mergeSort(float [], int, int);
	void merge(float *, int, int);

	void diff();
	void doorOpen();
	void printDoor();

};

