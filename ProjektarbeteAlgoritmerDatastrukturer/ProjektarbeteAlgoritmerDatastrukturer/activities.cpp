#include "pch.h"
#include <iostream>
#include "activities.h"
#include <fstream>
#include <string>
#include <algorithm>

// Konstruktor. H�mtar in all data till vektorerna EN g�ng! N�r programmet startar.
activities::activities()
{
	indata(); // H�mtar in data fr�n fil
	average(); // r�knar ut alla medelv�rden
	mould();	// r�knar ut m�gelrisk
	metrologisk(); // r�knar ut metrologisk h�st/vinter
	averageIndoorTemp();
	doorOpen(); // balkongd�rren �r �ppen eller ej varje timme.
	hoursOpen(); // r�knar ut hur l�nge det var �ppet.
	diff(); // r�knar ut hur mycket diff det �r mellan dagar inne/ute.
}

// Deconstruktor, tar bort vectorerna n�r programmet st�ngs.
activities::~activities()
{

	for (int i = 0; i < sizeVectorInside; i++)
	{
		delete inside[i];
	}

	for (int i = 0; i < sizeVectorOutside; i++)
	{
		delete outside[i];
	}

	for (int i = 0; i < Average.size() ; i++)
	{
		delete Average[i];
	}


}

// Metod som h�mtar in data fr�n fil.
void activities::indata()
{
	// �ppnar filen "DataUseThis.csv"
	std::ifstream datafile("tempdata4.csv");
	//std::ifstream datafile("DataUseThis.csv");
	std::string tempstring, date, time, place, temp, moist;

	// Om filen �r �ppen, h�mta in datum, tid osv. till dess variabler.
	if (datafile.is_open())
	{
		while (!datafile.eof()) // forts�tt s� l�nge som det finns n�got i datan.
		{

			getline(datafile, date, ' ');	// forts�tt till f�rsta mellanslag. Tilldela datan till variabeln date.
			getline(datafile, time, ',');	// forts�tt till kommatecken
			getline(datafile, place, ',');  // osv..
			getline(datafile, temp, ',');
			getline(datafile, moist);

			// Kollar om det �r inne eller ute. Skickar in datan i vektorn.
			if (place == "Inne")
			{
				// g�r samtidigt om vissa variabler fr�n string till float.
				inside.push_back(new theData(date, time, place, stof(temp), stof(moist)));

			}
			else
			{
				outside.push_back(new theData(date, time, place, stof(temp), stof(moist)));

			}
		}
		// St�nger filen n�r man �r f�rdig med den.
		datafile.close(); 
	}
	else
	{
		std::cout << "File not found" << std::endl;
	}

	// g�r om vektorns storlek till en variabel. s� man slipper anropa .size hela tiden.
	sizeVectorInside = inside.size(); 
	sizeVectorOutside = outside.size();

};


// Sortera efter medeltemperatur. Till Stable_sort funktion.
bool hej(const AverageAll *a, const AverageAll *b)
{
	return a->get_averageTemp() < b->get_averageTemp();
};

// Sortera efter medelfuktighet. Till Stable_sort funktion.
bool sortHum(const AverageAll *a, const AverageAll *b)
{
	return a->get_averageMoist() < b->get_averageMoist();
};

// sortera efter m�gelrisk. Till Stable_sort funktion.
bool sortMould(const AverageAll *a, const AverageAll *b)
{
	return a->get_mouldRisk() < b->get_mouldRisk();
};

// sortera efter st�rst risk. Till Stable_sort funktion.
bool sortRisk(const AverageAll *a, const AverageAll *b)
{
	return a->get_diff() < b->get_diff();

};

// sortera efter d�rr �ppen l�ngst/minst.
bool sortDoor(const theData *a, const theData *b)
{
	return a->get_hoursOpened() < b->get_hoursOpened();

};


// Huvudmenyn.
void activities::menuOne()
{
	bool goMenu = false;
	do {
	std::cout << " MAIN MENU\n [1] Indoor data\n [2] Outdoor data\n [4] Temperature differenses Indoor/Outdoor \n [5] When is the balcony door open?\n [6] Exit" << std::endl;
	std::cin >> answer;
	answer = toupper(answer[0]);

		switch (answer[0])
		{
		case '1':
			menuInside();
			goMenu = true;
			break;
		case '2':
			menuOutside();
			goMenu = true;
			break;
		case '4':
			std::stable_sort(Average.begin(), Average.end(), sortRisk); // Sorterar medeltemp
			printdiff();
			break;
		case '5':
			std::stable_sort(open.begin(), open.end(), sortDoor); // Sorterar efter d�rr �ppen l�ngst tid.
			printDoor();
			break;
		case '6':
			goMenu = true;
			break;
		default:
			break;
		}
	} while (!goMenu);
}

// Undermeny inomhus
void activities::menuInside()
{
	bool goMenuIn = false;
	do {
		std::cout << " INDOOR MENU\n [1] Search for average temp by date \n [2] Sort by temp \n [3] Sort by humidity \n [4] Mouldrisk \n [5] Go back" << std::endl;
		std::cin >> answer;
		answer = toupper(answer[0]);
		switch (answer[0])
		{
		case '1':
			std::cout << " Please insert date (in this format 2016-10-01): ";
			std::cin >> searchDate;
			std::cout << " You searched for: " << searchDate << std::endl;
			searchDay(searchDate, "In");
			break;
		case '2':
			std::stable_sort(Average.begin(), Average.end(), hej); // Sorterar medeltemp
			printAverage("In"); // printar medelv�rden inne.
			break;
		case '3':
			std::stable_sort(Average.begin(), Average.end(), sortHum); // Sorterar medelfuktighet
			printAverage("In"); // printar medelv�rden inne.
			break;
		case '4':
			std::stable_sort(Average.begin(), Average.end(), sortMould); // Sorterar efter m�gelrisk
			printMould("In");
			break;
		case '5':
			menuOne();
			goMenuIn = true;
			break;
		default:
			break;
		}
	} while (!goMenuIn);
}

// Undermeny utomhus
void activities::menuOutside()
{
	bool goMenuOut = false;
	do {
		std::cout << " OUTDOOR MENU\n [1] Autum/Winter \n [2] Search for average temp by date\n [3] Sort by temp  \n [4] Sort by humidity \n [5] Mouldrisk \n [6] Go back" << std::endl;
		std::cin >> answer;
		answer = toupper(answer[0]);
		switch (answer[0])
		{
		case '1':
			std::cout << " Meteorological autumn starts : " << metro << std::endl;
			std::cout << " Meteorological winter starts : " << metroW << std::endl;
		
			break;
		case '2':
			std::cout << " Please insert date (in this format 2016-10-01): ";
			std::cin >> searchDate;
			std::cout << " You searched for: " << searchDate << std::endl;
			searchDay(searchDate, "Out");	
			break;
		case '3':
			std::stable_sort(Average.begin(), Average.end(), hej); // Sorterar medeltemp
			printAverage("Out"); // Printar medeltemp ute.
			break;
		case '4':
			std::stable_sort(Average.begin(), Average.end(), sortHum); // Sorterar medelfuktighet
			printAverage("Out"); // Printar medeltemp ute.
		case '5':
			std::stable_sort(Average.begin(), Average.end(), sortMould); // Sorterar efter m�gelrisk
			printMould("Out");
			break;
		case '6':
			menuOne();
			goMenuOut = true;
			break;
		default:
			break;
		}
	} while (!goMenuOut);
}

// Hittar r�tt day, returnerar medeltemp/medelfuktighet
void activities::searchDay(std::string date, std::string enviroment)
{
	checkDate = 0;
	float dummy = 0;

	// Medel temperaturen INNE
	if (enviroment == "In")
	{
		for (int i = 0; i < Average.size(); i++)
		{
			if (Average[i]->get_a_date() == date && Average[i]->get_indoor() == true) 
			{
				dummy =  Average[i]->get_averageTemp();
				checkDate = 1;
			}
		}

	}

	// Medel temperaturen UTE
	else if (enviroment == "Out")
	{
		for (int i = 0; i < Average.size(); i++)
		{
			if (Average[i]->get_a_date() == date && Average[i]->get_indoor() == false)
			{
				dummy = Average[i]->get_averageTemp();
				checkDate = 1;
			}
			
		}
	}

	std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
	if (checkDate != 0)
	{
		std::cout << " Average temperature: " << dummy << std::endl;
	}
	else
	{
		std::cout << " Date not found." << std::endl;
	}
	std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;



}

// R�knar ut medelv�rde.
void activities::average()
{
	/* medelv�rden inne */

	std::string datum, tid, timmeT;
	int counter = 1, test1 = 0;			// r�knar antal
	float summa = inside[0]->get_temp(); // summan av temp.
	float summa2 = inside[0]->get_moist(); // summan av fuktighet.

	float summaDoor = inside[0]->get_temp(); // till medeltemp per timme
	float summaMdoor = inside[0]->get_moist(); // till medelfukt per timme
	int countDoor = 1;


	for (int i = 0; i < sizeVectorInside - 2 ; i++) // kollar hela vectorn.
	{
		// om datumen brevid varandra �r samma.
		if (inside[i]->get_date() == inside[i + 1]->get_date())
		{
			// medeltemp.
			summa = summa + inside[i + 1]->get_temp();
				
			// medelfuktighet.
			summa2 = summa2 + inside[i + 1]->get_moist();
			counter++;
			test1++;


			// till balkongd�rr. R�knar ut ett medelv�rde varje timme.
			if (inside[i]->get_time().substr(0, 2) == inside[i + 1]->get_time().substr(0, 2)) // om det �r samma timme.
			{
				// till medeltemp (timme)
				summaDoor = summaDoor + inside[i + 1]->get_temp(); // plussar p� alla med samma timme.

				// till medelfukt (timme)
				summaMdoor = summaMdoor + inside[i + 1]->get_moist();
				countDoor++;
				
			}
			else
			{
				// pushar in medelv�rden per timme.
				timmeT = inside[i]->get_time().substr(0, 2); // h�mtar ut vilken timme det �r.
				datum = inside[i]->get_date();
				doorIn.push_back(new theData(datum, (summaDoor/countDoor), (summaMdoor/countDoor),  timmeT));

				// nollst�ller v�rden.
				summaDoor = inside[i + 1]->get_temp();
				summaMdoor = inside[i + 1]->get_moist();
				countDoor = 1;
			}
		}

		else 
		{
			// Skickar in i door vectorn. f�rsta timmen, ny dag.
			datum = inside[i]->get_date();
			timmeT = inside[i]->get_time().substr(0, 2); // h�mtar ut vilken timme det �r.
			doorIn.push_back(new theData(datum, (summaDoor / countDoor), (summaMdoor / countDoor), timmeT));

			// skickar in medelv�rdena i vektorn.
			Average.push_back(new AverageAll(datum, (summa / counter), (summa2 / counter), true)); 

			// nollst�ller v�rden balkongd�rr.
			summaDoor = inside[i + 1]->get_temp();
			summaMdoor = inside[i + 1]->get_moist();
			countDoor = 1;

			// nollst�ller alla v�rden
			summa = 0, summa2 = 0; 
			summa = inside[i + 1]->get_temp();
			summa2 = inside[i + 1]->get_moist();
			counter = 1;
			test1++;
		}
	}


	// skickar in sista dagen.
	datum = inside[test1]->get_date();
	Average.push_back(new AverageAll(datum, (summa / counter), (summa2 / counter), true)); 

	timmeT = inside[test1]->get_time().substr(0, 2); // h�mtar ut vilken timme det �r.
	doorIn.push_back(new theData(datum, (summaDoor / countDoor), (summaMdoor / countDoor), timmeT));


	/* medelv�rden ute */
	int test = 0;
	counter = 1;
	summa = outside[0]->get_temp();
	summa2 = outside[0]->get_moist();

	summaDoor = outside[0]->get_temp(); // till medeltemp per timme
	summaMdoor = outside[0]->get_moist(); // till medelfukt per timme
	countDoor = 1;


	for (int i = 0; i < sizeVectorOutside - 2; i++) // kollar hela vectorn.
	{
		// om datumen brevid varandra �r samma.
		if (outside[i]->get_date() == outside[i + 1]->get_date())
		{
			// medeltemp.
			summa = summa + outside[i + 1]->get_temp();

			// medelfuktighet.
			summa2 = summa2 + outside[i + 1]->get_moist();
			counter++;
			test++;

			// till balkongd�rr. R�knar ut ett medelv�rde varje timme.
			if (outside[i]->get_time().substr(0, 2) == outside[i + 1]->get_time().substr(0, 2)) // om det �r samma timme.
			{
				// till medeltemp (timme)
				summaDoor = summaDoor + outside[i + 1]->get_temp(); // plussar p� alla med samma timme.

				// till medelfukt (timme)
				summaMdoor = summaMdoor + outside[i + 1]->get_moist();
				countDoor++;

			}
			else
			{
				// pushar in medelv�rden per timme.
				timmeT = outside[i]->get_time().substr(0, 2); // h�mtar ut vilken timme det �r.
				datum = outside[i]->get_date();
				doorOut.push_back(new theData(datum, (summaDoor / countDoor), (summaMdoor / countDoor), timmeT));

				// nollst�ller alla v�rden.
				summaDoor = outside[i + 1]->get_temp();
				summaMdoor = outside[i + 1]->get_moist();
				countDoor = 1;
			}
		}

		else
		{
			// Skickar in i door vectorn. f�rsta timmen, ny dag.
			datum = outside[i]->get_date();
			timmeT = outside[i]->get_time().substr(0, 2); // h�mtar ut vilken timme det �r.
			doorOut.push_back(new theData(datum, (summaDoor / countDoor), (summaMdoor / countDoor), timmeT));

			// skickar in medelv�rdena i vektorn.
			datum = outside[i]->get_date();
			Average.push_back(new AverageAll(datum, (summa / counter), (summa2 / counter), false)); 

			// nollst�ller alla v�rden. balkongd�rr
			summaDoor = outside[i + 1]->get_temp();
			summaMdoor = outside[i + 1]->get_moist();
			countDoor = 1;

			// nollst�ller alla v�rden
			summa = 0, summa2 = 0;
			summa = outside[i + 1]->get_temp();
			summa2 = outside[i + 1]->get_moist();
			counter = 1;
			test++;
		}
	}

	// Sista dagen
	datum = outside[test]->get_date();
	Average.push_back(new AverageAll(datum, (summa / counter), (summa2 / counter), false));

	timmeT = outside[test]->get_time().substr(0, 2); // h�mtar ut vilken timme det �r.
	doorOut.push_back(new theData(datum, (summaDoor / countDoor), (summaMdoor / countDoor), timmeT));
}

// Printar medeltemp alla dagar.
void activities::printAverage(std::string enviroment)
{
	int count = 0;
	int counter = 0;

	if (enviroment == "In")
	{
		count = 0;
		std::cout << " ######################################## " << std::endl;
		std::cout << " HIGHEST:" << std::endl;
		std::cout << " ######################################## " << std::endl;
		for (int i = Average.size() - 1; i > 0; i--)
		{
			if (Average[i]->get_indoor() == true && count < 5)
			{
				std::cout << " Indoor ";
				std::cout << Average[i]->get_a_date() << "\n Average temp: "
					<< Average[i]->get_averageTemp() << std::endl;
				std::cout << " Average humidity: " << Average[i]->get_averageMoist() << std::endl;
				std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
				count++;
			}
		}

		count = 0;

		std::cout << " ######################################## " << std::endl;
		std::cout << " LOWEST: " << std::endl;
		std::cout << " ######################################## " << std::endl;
		for (int i = 0; i < Average.size(); i++)
		{
			if (Average[i]->get_indoor() == true && count < 5)
			{
				std::cout << " Indoor ";
				std::cout << Average[i]->get_a_date() << "\n Average temp: "
					<< Average[i]->get_averageTemp() << std::endl;
				std::cout << " Average humidity: " << Average[i]->get_averageMoist() << std::endl;
				std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
				count++;
			}
		}
	}

	//utomhus
	else if (enviroment == "Out")
	{
		counter = 0;
		std::cout << " ######################################## " << std::endl;
		std::cout << " HIGHEST:" << std::endl;
		std::cout << " ######################################## " << std::endl;
		for (int i = Average.size() - 1; i > 0; i--)
		{
			if (Average[i]->get_indoor() == false && counter < 5)
			{
				std::cout << " Outdoor ";
				std::cout << Average[i]->get_a_date() << "\n Average temp: "
					<< Average[i]->get_averageTemp() << std::endl;
				std::cout << " Average humidity: " << Average[i]->get_averageMoist() << std::endl;
				std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
				counter++;
			}
		}

		counter = 0;
		std::cout << " ######################################## " << std::endl;
		std::cout << " LOWEST: " << std::endl;
		std::cout << " ######################################## " << std::endl;

		for (int i = 0; i < Average.size(); i++)
		{
			if (Average[i]->get_indoor() == false && counter < 5)
			{
				std::cout << " Outdoor ";
				std::cout << Average[i]->get_a_date() << "\n Average temp: "
					<< Average[i]->get_averageTemp() << std::endl;
				std::cout << " Average humidity: " << Average[i]->get_averageMoist() << std::endl;
				std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
				counter++;
			}
		}
	}
}

// Metrologisk h�st/vinter
void activities::metrologisk()
{
	/*Datum f�r metrologisk h�st*/
	for (int i = 0; i < Average.size() - 4; i++) // s� den inte j�mf�r den sista med ingenting.
	{
		// Om m�naden �r m�nad 8 eller senare.
		if (Average[i]->get_a_date()[5] == 0 && Average[0]->get_a_date()[6] > 7 || Average[i]->get_a_date()[5] != 0)
		{
			if (Average[i]->get_indoor() == false && Average[i]->get_averageTemp() < 10 &&
				Average[i + 1]->get_indoor() == false && Average[i + 1]->get_averageTemp() < 10 &&
				Average[i + 2]->get_indoor() == false && Average[i + 2]->get_averageTemp() < 10 &&
				Average[i + 3]->get_indoor() == false && Average[i + 3]->get_averageTemp() < 10 &&
				Average[i + 4]->get_indoor() == false && Average[i + 4]->get_averageTemp() < 10)
			{
				metro = Average[i]->get_a_date();
				i = Average.size() + 5;
			}
		}
	}

	/*Datum f�r metrologisk vinter*/
	int check = 0;

	for (int x = 0; x < Average.size() - 4; x++) // s� den inte j�mf�r den sista med ingenting.
	{
		// Kollar f�rst alla m�nader 10-12. 
		if (Average[x]->get_a_date()[5] != 0)
		{
			if (Average[x]->get_indoor() == false && Average[x]->get_averageTemp() < 0 &&
				Average[x + 1]->get_indoor() == false && Average[x + 1]->get_averageTemp() < 0 &&
				Average[x + 2]->get_indoor() == false && Average[x + 2]->get_averageTemp() < 0 &&
				Average[x + 3]->get_indoor() == false && Average[x + 3]->get_averageTemp() < 0 &&
				Average[x + 4]->get_indoor() == false && Average[x + 4]->get_averageTemp() < 0)
			{
				metroW = Average[x]->get_a_date();
				x = Average.size() + 5; // stoppar loopen.
				check = 1; // stoppar s�kandet.
			}
		}
	}

	if (check == 0) // bara om den inte hittade n�got i f�rsta s�kningen.
	{
		for (int x = 0; x < Average.size() - 4; x++) // s� den inte j�mf�r den sista med ingenting.
		{
			// Kollar sedan m�naderna 1-9. Och �ven �verg�ngen mellan december-januari.
			if (Average[x]->get_a_date()[5] == '1' && Average[x]->get_a_date()[6] == '2' || Average[x]->get_a_date()[5] == '0')
			{

				if (Average[x]->get_indoor() == false && Average[x]->get_averageTemp() < 0 &&
					Average[x + 1]->get_indoor() == false && Average[x + 1]->get_averageTemp() < 0 &&
					Average[x + 2]->get_indoor() == false && Average[x + 2]->get_averageTemp() < 0 &&
					Average[x + 3]->get_indoor() == false && Average[x + 3]->get_averageTemp() < 0 &&
					Average[x + 4]->get_indoor() == false && Average[x + 4]->get_averageTemp() < 0 )
				{
					metroW = Average[x]->get_a_date();
					x = Average.size() + 5;
					check = 1;
				}
			}
		}
	}
}

// R�knar ut m�gelrisk.
void activities::mould()
{
	float fuktighet = 0, temp, mouldLine, mouldRisk;

	for (int i = 0; i < Average.size(); i++)
	{
		// H�mtar in medeltemp.
		temp = Average[i]->get_averageTemp();

		// r�knar ut m�gelgr�nsen utefter en formel
		mouldLine = -0.0015 * pow(temp, 3) + 0.1193 * pow(temp, 2) - (2.9878 * temp) + 102.96;

		// h�mtar in fuktighet.
		fuktighet = Average[i]->get_averageMoist();

		// m�gelrisken blir
		mouldRisk = fuktighet - mouldLine;

		// s�tter m�gelrisken f�r alla datum.
		Average[i]->set_mouldRisk(mouldRisk);

		// kollar om m�gelrisken var h�g eller l�g.
		if (fuktighet > mouldLine)
		{
			Average[i]->set_mouldBool("high");
		}
		else
		{
			Average[i]->set_mouldBool("low");
		}
	}
}

// printar ut m�gelrisk.
void activities::printMould(std::string enviroment)
{
	int count = 0;
	int counter = 0;

	if (enviroment == "In")
	{
		count = 0;
		std::cout << " ######################################## " << std::endl;
		std::cout << " HIGHEST:" << std::endl;
		std::cout << " ######################################## " << std::endl;
		for (int i = Average.size() - 1; i > 0; i--)
		{
			if (Average[i]->get_indoor() == true && count < 5)
			{
				std::cout << " Indoor ";
				std::cout << Average[i]->get_a_date() << std::endl;
				std::cout << " Mouldrisk: " << Average[i]->get_mouldBool() << std::endl;
				std::cout << " " << Average[i]->get_mouldRisk() << std::endl;
				std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
				count++;
			}
		}

		count = 0;

		std::cout << " ######################################## " << std::endl;
		std::cout << " LOWEST: " << std::endl;
		std::cout << " ######################################## " << std::endl;
		for (int i = 0; i < Average.size(); i++)
		{
			if (Average[i]->get_indoor() == true && count < 5)
			{
				std::cout << " Indoor ";
				std::cout << Average[i]->get_a_date() << std::endl;
				std::cout << " Mouldrisk: " << Average[i]->get_mouldBool() << std::endl;
				std::cout << " " << Average[i]->get_mouldRisk() << std::endl;
				std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
				count++;
			}
		}
	}

	//utomhus
	else if (enviroment == "Out")
	{
		counter = 0;
		std::cout << " ######################################## " << std::endl;
		std::cout << " HIGHEST:" << std::endl;
		std::cout << " ######################################## " << std::endl;
		for (int i = Average.size() - 1; i > 0; i--)
		{
			if (Average[i]->get_indoor() == false && counter < 5)
			{
				std::cout << " Outdoor ";
				std::cout << Average[i]->get_a_date() << std::endl;
				std::cout << " Mouldrisk: " << Average[i]->get_mouldBool() << std::endl;
				std::cout << " " << Average[i]->get_mouldRisk() << std::endl;
				std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
				counter++;
			}
		}

		counter = 0;
		std::cout << " ######################################## " << std::endl;
		std::cout << " LOWEST: " << std::endl;
		std::cout << " ######################################## " << std::endl;

		for (int i = 0; i < Average.size(); i++)
		{
			if (Average[i]->get_indoor() == false && counter < 5)
			{
				std::cout << " Outdoor ";
				std::cout << Average[i]->get_a_date() << std::endl;
				std::cout << " Mouldrisk: " << Average[i]->get_mouldBool() << std::endl;
				std::cout << " " << Average[i]->get_mouldRisk() << std::endl;
				std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
				counter++;
			}
		}
	}
}

// r�knar ut diffen, setter diff.
void activities::diff()
{
	// Sortering p� d� inne - och ytterMEDELtemperaturen skiljt sig mest och minst.
	int x = 0, i = 1;
	float dummy = 0;

	while (x < Average.size())
	{
		while (i < Average.size())
		{
			// Om det �r samma datum och ena �r inne och andra �r ute.
			if (Average[x]->get_a_date() == Average[i]->get_a_date()
				&& Average[x]->get_indoor() == true && Average[i]->get_indoor() == false)
			{
				dummy = Average[x]->get_averageTemp() - Average[i]->get_averageTemp();
				Average[x]->set_tempDiff(dummy);
				Average[i]->set_tempDiff(dummy);
			}
			i++;
		}
		x++;
		i = x + 1;
	}


}

// printar ut h�gsta/l�gsta diff 
void activities::printdiff()
{
	std::cout << " --- --- --- --- --- --- --- --- --- --- --- --- --- --" << std::endl;
	std::cout << " Higest differens between average temp indoor/outdoor: " << std::endl;
	std::cout << " --- --- --- --- --- --- --- --- --- --- --- --- --- --" << std::endl;

	for (int i = Average.size() - 1; i > Average.size() - 11; i--)
	{
		std::cout << " " << Average[i]->get_a_date() << " || " << Average[i]->get_diff() << " " << std::endl;
		i--;
	}

	std::cout << " --- --- --- --- --- --- --- --- --- --- --- --- --- --" << std::endl;
	std::cout << " Lowest differens between average temp indoor/outdoor: " << std::endl;
	std::cout << " --- --- --- --- --- --- --- --- --- --- --- --- --- --" << std::endl;

	for (int i = 0; i < 10; i++)
	{
		std::cout << " " << Average[i]->get_a_date() << " || " << Average[i]->get_diff() << " " << std::endl;
		i++;
	}

	std::cout << "\n";

}

// kollar om balkongd�rren �r �ppen eller ej varje timme.
void activities::doorOpen() {

	double diffTemp = 0, diffHum = 0, check;

	for (int x = 0; x < doorIn.size(); x++) // kollar hela ute vektorn.
	{
		// TILL ALGORITHM NR 2
		check = IndoorAverageTemp - doorIn[0]->get_temp();

		// om skiljer sig fr�n det den brukar vara.
		if (check > 2 && check < -2)
		{
			doorIn[x]->set_OpenNr2(true);
		}

		for (int i = 0; i < doorOut.size(); i++) // Tar alla innev�rden
		{
			if (doorIn[x]->get_time() == doorOut[i]->get_time()) // om det �r samma timme
			{
				// j�mf�r temp och fuktighet

				diffHum = doorIn[x]->get_moist() - doorOut[i]->get_moist();
				diffTemp = doorIn[x]->get_temp() - doorOut[i]->get_temp();
	
				// Om det �r liten diff mellan temp,
				if(diffTemp < 2 && diffTemp > -2)
				{
					// om det �r liten diff fuktighet.
					if (diffHum < 2 && diffHum > -2)
					{
						// det �r �ppet
						doorIn[x]->set_doorClosed("A");
					}
				}
			}
		}
	}
}

// visar om balkongd�rren �r �ppen
void activities::hoursOpen()
{
	std::string openedDate, openedTime, closedDate, closedTime;
	int countTime = 0, count2 = 0;
	
	for (int i = 0; i < doorIn.size(); i++)
	{
		if (doorIn[i]->get_doorClosed() == "A") // om d�rren �r �ppen
		{
			countTime++;

			// n�r den �ppnades.
			openedDate = doorIn[i]->get_date();
			openedTime = doorIn[i]->get_time();

			while (doorIn[i + 1]->get_doorClosed() == "A") // s� l�nge som d�rren �r �ppen.
			{
				countTime++;
				i++;
			}
		}
		else
		{
			if (countTime != 0)
			{
				//// n�r den st�ngdes.
				closedDate = doorIn[i]->get_date();
				closedTime = doorIn[i]->get_time();

				// datum, tid, datum, tid, antal timmar.
				open.push_back(new theData(openedDate, openedTime, countTime, closedDate, closedTime));

			}
		
			countTime = 0;
		}

	}
}

// printar ut n�r d�rren har varit �ppen
void activities::printDoor()
{
	std::cout << " --- --- --- --- --- --- --- --- --- -- -- " << std::endl;
	std::cout << " ALGORITHM ONE SAID" << std::endl;
	std::cout << " The balcony door was opened the most at: " << std::endl;
	std::cout << " --- --- --- --- --- --- --- --- --- -- -- " << std::endl;

	for (int i = open.size()-1; i > open.size() - 6; i--)
	{
		std::cout << " door opened at: " << open[i]->get_date() << " " << open[i]->get_time() << ":00 ";
		std::cout << " hours open: " << open[i]->get_hoursOpened();
		std::cout << " door closed at: " << open[i]->get_ClosedDate() << " " << open[i]->get_ClosedTime()<< ":00 " << std::endl;
	}

	std::cout << "\n";

	std::cout << " --- --- --- --- --- --- --- --- --- -- -- " << std::endl;
	std::cout << " ALGORITHM TWO SAID" << std::endl;
	std::cout << " The balcony door was opened the most at: " << std::endl;
	std::cout << " --- --- --- --- --- --- --- --- --- -- -- " << std::endl;

	std::cout << "\n";
}

// r�knar ut vad temperaturen vanligtvis �r inne.
void activities::averageIndoorTemp()
{
	float summa = 0;
	int count = 0;

	for (int i = 0; i < Average.size(); i++)
	{
		if (Average[i]->get_indoor() == true)
		{

			summa = summa + Average[i]->get_averageTemp();
			count++;

		}
	}

	IndoorAverageTemp = (summa / count);

}