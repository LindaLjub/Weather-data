#include "pch.h"
#include <iostream>
#include "activities.h"
#include <fstream>
#include <string>
#include <algorithm>

// Konstruktor. Hämtar in all data till vektorerna EN gång! När programmet startar.
activities::activities()
{
	indata(); // Hämtar in data från fil
	average(); // räknar ut alla medelvärden
	mould();	// räknar ut mögelrisk
	metrologisk(); // räknar ut metrologisk höst/vinter
	averageIndoorTemp();
	doorOpen(); // balkongdörren är öppen eller ej varje timme.
	hoursOpen(); // räknar ut hur länge det var öppet.
	diff(); // räknar ut hur mycket diff det är mellan dagar inne/ute.
}

// Deconstruktor, tar bort vectorerna när programmet stängs.
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

// Metod som hämtar in data från fil.
void activities::indata()
{
	// Öppnar filen "DataUseThis.csv"
	std::ifstream datafile("tempdata4.csv");
	//std::ifstream datafile("DataUseThis.csv");
	std::string tempstring, date, time, place, temp, moist;

	// Om filen är öppen, hämta in datum, tid osv. till dess variabler.
	if (datafile.is_open())
	{
		while (!datafile.eof()) // fortsätt så länge som det finns något i datan.
		{

			getline(datafile, date, ' ');	// fortsätt till första mellanslag. Tilldela datan till variabeln date.
			getline(datafile, time, ',');	// fortsätt till kommatecken
			getline(datafile, place, ',');  // osv..
			getline(datafile, temp, ',');
			getline(datafile, moist);

			// Kollar om det är inne eller ute. Skickar in datan i vektorn.
			if (place == "Inne")
			{
				// gör samtidigt om vissa variabler från string till float.
				inside.push_back(new theData(date, time, place, stof(temp), stof(moist)));

			}
			else
			{
				outside.push_back(new theData(date, time, place, stof(temp), stof(moist)));

			}
		}
		// Stänger filen när man är färdig med den.
		datafile.close(); 
	}
	else
	{
		std::cout << "File not found" << std::endl;
	}

	// gör om vektorns storlek till en variabel. så man slipper anropa .size hela tiden.
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

// sortera efter mögelrisk. Till Stable_sort funktion.
bool sortMould(const AverageAll *a, const AverageAll *b)
{
	return a->get_mouldRisk() < b->get_mouldRisk();
};

// sortera efter störst risk. Till Stable_sort funktion.
bool sortRisk(const AverageAll *a, const AverageAll *b)
{
	return a->get_diff() < b->get_diff();

};

// sortera efter dörr öppen längst/minst.
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
			std::stable_sort(open.begin(), open.end(), sortDoor); // Sorterar efter dörr öppen längst tid.
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
			printAverage("In"); // printar medelvärden inne.
			break;
		case '3':
			std::stable_sort(Average.begin(), Average.end(), sortHum); // Sorterar medelfuktighet
			printAverage("In"); // printar medelvärden inne.
			break;
		case '4':
			std::stable_sort(Average.begin(), Average.end(), sortMould); // Sorterar efter mögelrisk
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
			std::stable_sort(Average.begin(), Average.end(), sortMould); // Sorterar efter mögelrisk
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

// Hittar rätt day, returnerar medeltemp/medelfuktighet
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

// Räknar ut medelvärde.
void activities::average()
{
	/* medelvärden inne */

	std::string datum, tid, timmeT;
	int counter = 1, test1 = 0;			// räknar antal
	float summa = inside[0]->get_temp(); // summan av temp.
	float summa2 = inside[0]->get_moist(); // summan av fuktighet.

	float summaDoor = inside[0]->get_temp(); // till medeltemp per timme
	float summaMdoor = inside[0]->get_moist(); // till medelfukt per timme
	int countDoor = 1;


	for (int i = 0; i < sizeVectorInside - 2 ; i++) // kollar hela vectorn.
	{
		// om datumen brevid varandra är samma.
		if (inside[i]->get_date() == inside[i + 1]->get_date())
		{
			// medeltemp.
			summa = summa + inside[i + 1]->get_temp();
				
			// medelfuktighet.
			summa2 = summa2 + inside[i + 1]->get_moist();
			counter++;
			test1++;


			// till balkongdörr. Räknar ut ett medelvärde varje timme.
			if (inside[i]->get_time().substr(0, 2) == inside[i + 1]->get_time().substr(0, 2)) // om det är samma timme.
			{
				// till medeltemp (timme)
				summaDoor = summaDoor + inside[i + 1]->get_temp(); // plussar på alla med samma timme.

				// till medelfukt (timme)
				summaMdoor = summaMdoor + inside[i + 1]->get_moist();
				countDoor++;
				
			}
			else
			{
				// pushar in medelvärden per timme.
				timmeT = inside[i]->get_time().substr(0, 2); // hämtar ut vilken timme det är.
				datum = inside[i]->get_date();
				doorIn.push_back(new theData(datum, (summaDoor/countDoor), (summaMdoor/countDoor),  timmeT));

				// nollställer värden.
				summaDoor = inside[i + 1]->get_temp();
				summaMdoor = inside[i + 1]->get_moist();
				countDoor = 1;
			}
		}

		else 
		{
			// Skickar in i door vectorn. första timmen, ny dag.
			datum = inside[i]->get_date();
			timmeT = inside[i]->get_time().substr(0, 2); // hämtar ut vilken timme det är.
			doorIn.push_back(new theData(datum, (summaDoor / countDoor), (summaMdoor / countDoor), timmeT));

			// skickar in medelvärdena i vektorn.
			Average.push_back(new AverageAll(datum, (summa / counter), (summa2 / counter), true)); 

			// nollställer värden balkongdörr.
			summaDoor = inside[i + 1]->get_temp();
			summaMdoor = inside[i + 1]->get_moist();
			countDoor = 1;

			// nollställer alla värden
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

	timmeT = inside[test1]->get_time().substr(0, 2); // hämtar ut vilken timme det är.
	doorIn.push_back(new theData(datum, (summaDoor / countDoor), (summaMdoor / countDoor), timmeT));


	/* medelvärden ute */
	int test = 0;
	counter = 1;
	summa = outside[0]->get_temp();
	summa2 = outside[0]->get_moist();

	summaDoor = outside[0]->get_temp(); // till medeltemp per timme
	summaMdoor = outside[0]->get_moist(); // till medelfukt per timme
	countDoor = 1;


	for (int i = 0; i < sizeVectorOutside - 2; i++) // kollar hela vectorn.
	{
		// om datumen brevid varandra är samma.
		if (outside[i]->get_date() == outside[i + 1]->get_date())
		{
			// medeltemp.
			summa = summa + outside[i + 1]->get_temp();

			// medelfuktighet.
			summa2 = summa2 + outside[i + 1]->get_moist();
			counter++;
			test++;

			// till balkongdörr. Räknar ut ett medelvärde varje timme.
			if (outside[i]->get_time().substr(0, 2) == outside[i + 1]->get_time().substr(0, 2)) // om det är samma timme.
			{
				// till medeltemp (timme)
				summaDoor = summaDoor + outside[i + 1]->get_temp(); // plussar på alla med samma timme.

				// till medelfukt (timme)
				summaMdoor = summaMdoor + outside[i + 1]->get_moist();
				countDoor++;

			}
			else
			{
				// pushar in medelvärden per timme.
				timmeT = outside[i]->get_time().substr(0, 2); // hämtar ut vilken timme det är.
				datum = outside[i]->get_date();
				doorOut.push_back(new theData(datum, (summaDoor / countDoor), (summaMdoor / countDoor), timmeT));

				// nollställer alla värden.
				summaDoor = outside[i + 1]->get_temp();
				summaMdoor = outside[i + 1]->get_moist();
				countDoor = 1;
			}
		}

		else
		{
			// Skickar in i door vectorn. första timmen, ny dag.
			datum = outside[i]->get_date();
			timmeT = outside[i]->get_time().substr(0, 2); // hämtar ut vilken timme det är.
			doorOut.push_back(new theData(datum, (summaDoor / countDoor), (summaMdoor / countDoor), timmeT));

			// skickar in medelvärdena i vektorn.
			datum = outside[i]->get_date();
			Average.push_back(new AverageAll(datum, (summa / counter), (summa2 / counter), false)); 

			// nollställer alla värden. balkongdörr
			summaDoor = outside[i + 1]->get_temp();
			summaMdoor = outside[i + 1]->get_moist();
			countDoor = 1;

			// nollställer alla värden
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

	timmeT = outside[test]->get_time().substr(0, 2); // hämtar ut vilken timme det är.
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

// Metrologisk höst/vinter
void activities::metrologisk()
{
	/*Datum för metrologisk höst*/
	for (int i = 0; i < Average.size() - 4; i++) // så den inte jämför den sista med ingenting.
	{
		// Om månaden är månad 8 eller senare.
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

	/*Datum för metrologisk vinter*/
	int check = 0;

	for (int x = 0; x < Average.size() - 4; x++) // så den inte jämför den sista med ingenting.
	{
		// Kollar först alla månader 10-12. 
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
				check = 1; // stoppar sökandet.
			}
		}
	}

	if (check == 0) // bara om den inte hittade något i första sökningen.
	{
		for (int x = 0; x < Average.size() - 4; x++) // så den inte jämför den sista med ingenting.
		{
			// Kollar sedan månaderna 1-9. Och även övergången mellan december-januari.
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

// Räknar ut mögelrisk.
void activities::mould()
{
	float fuktighet = 0, temp, mouldLine, mouldRisk;

	for (int i = 0; i < Average.size(); i++)
	{
		// Hämtar in medeltemp.
		temp = Average[i]->get_averageTemp();

		// räknar ut mögelgränsen utefter en formel
		mouldLine = -0.0015 * pow(temp, 3) + 0.1193 * pow(temp, 2) - (2.9878 * temp) + 102.96;

		// hämtar in fuktighet.
		fuktighet = Average[i]->get_averageMoist();

		// mögelrisken blir
		mouldRisk = fuktighet - mouldLine;

		// sätter mögelrisken för alla datum.
		Average[i]->set_mouldRisk(mouldRisk);

		// kollar om mögelrisken var hög eller låg.
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

// printar ut mögelrisk.
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

// räknar ut diffen, setter diff.
void activities::diff()
{
	// Sortering på då inne - och ytterMEDELtemperaturen skiljt sig mest och minst.
	int x = 0, i = 1;
	float dummy = 0;

	while (x < Average.size())
	{
		while (i < Average.size())
		{
			// Om det är samma datum och ena är inne och andra är ute.
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

// printar ut högsta/lägsta diff 
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

// kollar om balkongdörren är öppen eller ej varje timme.
void activities::doorOpen() {

	double diffTemp = 0, diffHum = 0, check;

	for (int x = 0; x < doorIn.size(); x++) // kollar hela ute vektorn.
	{
		// TILL ALGORITHM NR 2
		check = IndoorAverageTemp - doorIn[0]->get_temp();

		// om skiljer sig från det den brukar vara.
		if (check > 2 && check < -2)
		{
			doorIn[x]->set_OpenNr2(true);
		}

		for (int i = 0; i < doorOut.size(); i++) // Tar alla innevärden
		{
			if (doorIn[x]->get_time() == doorOut[i]->get_time()) // om det är samma timme
			{
				// jämför temp och fuktighet

				diffHum = doorIn[x]->get_moist() - doorOut[i]->get_moist();
				diffTemp = doorIn[x]->get_temp() - doorOut[i]->get_temp();
	
				// Om det är liten diff mellan temp,
				if(diffTemp < 2 && diffTemp > -2)
				{
					// om det är liten diff fuktighet.
					if (diffHum < 2 && diffHum > -2)
					{
						// det är öppet
						doorIn[x]->set_doorClosed("A");
					}
				}
			}
		}
	}
}

// visar om balkongdörren är öppen
void activities::hoursOpen()
{
	std::string openedDate, openedTime, closedDate, closedTime;
	int countTime = 0, count2 = 0;
	
	for (int i = 0; i < doorIn.size(); i++)
	{
		if (doorIn[i]->get_doorClosed() == "A") // om dörren är öppen
		{
			countTime++;

			// när den öppnades.
			openedDate = doorIn[i]->get_date();
			openedTime = doorIn[i]->get_time();

			while (doorIn[i + 1]->get_doorClosed() == "A") // så länge som dörren är öppen.
			{
				countTime++;
				i++;
			}
		}
		else
		{
			if (countTime != 0)
			{
				//// när den stängdes.
				closedDate = doorIn[i]->get_date();
				closedTime = doorIn[i]->get_time();

				// datum, tid, datum, tid, antal timmar.
				open.push_back(new theData(openedDate, openedTime, countTime, closedDate, closedTime));

			}
		
			countTime = 0;
		}

	}
}

// printar ut när dörren har varit öppen
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

// räknar ut vad temperaturen vanligtvis är inne.
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