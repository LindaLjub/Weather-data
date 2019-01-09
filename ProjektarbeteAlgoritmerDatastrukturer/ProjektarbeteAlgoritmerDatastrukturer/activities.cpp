#include "pch.h"
#include "activities.h"
#include <stdlib.h>

// Konstruktor. Hämtar in all data till vektorerna EN gång! När programmet startar.
activities::activities()
{
	indata();
	testAverage();
	mould();
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

	for (int i = 0; i < sizeVectorAverageIn; i++)
	{
		delete AverageInside[i];
	}

	for (int i = 0; i < sizeVectorAverageOut; i++)
	{
		delete AverageOutside[i];
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
		while (!datafile.eof())
		{

			getline(datafile, date, ' ');
			getline(datafile, time, ',');
			getline(datafile, place, ',');
			getline(datafile, temp, ',');
			getline(datafile, moist);

			// Kollar om det är inne eller ute. Skickar in datan i vektorn.
			if (place == "Inne")
			{
				inside.push_back(new theData(date, time, place, stof(temp), stof(moist)));

			}
			else
			{
				outside.push_back(new theData(date, time, place, stof(temp), stof(moist)));

			}
		}
		datafile.close(); // Stänger filen när man är färdig med den.
	}
	else
	{
		std::cout << "File not found" << std::endl;
	}

	sizeVectorInside = inside.size();
	sizeVectorOutside = outside.size();

};

// Huvudmenyn.
void activities::menuOne()
{

	bool goMenu = false;
	do {
	std::cout << " MAIN MENU\n [1] Indoor data\n [2] Outdoor data\n [3] Exit" << std::endl;
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
		case '3':
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
	double dummy;
	bool goMenuIn = false;
	do {
		std::cout << " INDOOR MENU\n [1] --\n [2] Search for average temp by date \n [3] Daily average temp & humidity \n [4] Go back" << std::endl;
		std::cin >> answer;
		answer = toupper(answer[0]);
		switch (answer[0])
		{
		case '1':
			break;
		case '2':
			std::cout << " Please insert date (in this format 2016-10-01): ";
			std::cin >> searchDate;
			std::cout << " You searched for: " << searchDate << std::endl;
			dummy = searchDay(searchDate, "In", "temp");
			
			std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
				if (dummy != 0)
				{
					std::cout << " Average temperature: " << dummy << std::endl;
				}
				else
				{
					std::cout << " Date not found.. " << std::endl;
				}
			std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;

			break;
		case '3':
			printAverage("In"); // printar medeltemp inne.
			break;
		case '4':
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
	double dummy;
	bool goMenuOut = false;
	do {
		std::cout << " OUTDOOR MENU\n [1] Autum/Winter \n [2] Search for average temp by date\n [3] Daily average temp & humidity \n [4] Mouldrisk \n [5] Go back" << std::endl;
		std::cin >> answer;
		answer = toupper(answer[0]);
		switch (answer[0])
		{
		case '1':
			metrologisk();
			break;
		case '2':
			std::cout << " Please insert date (in this format 2016-10-01): ";
			std::cin >> searchDate;
			std::cout << " You searched for: " << searchDate << std::endl;
			dummy = searchDay(searchDate, "Out", "temp");

			std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
				if (dummy != 0)
				{
					std::cout << " Average temperature: " << dummy << std::endl;
				}
				else
				{
					std::cout << " Date not found.. " << std::endl;
				}
			std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
		
			break;
		case '3':
			printAverage("Out"); // Printar medeltemp ute.
			break;
		case '4':
			printMould();
			break;
		case '5':
			menuOne();
			goMenuOut = true;
			break;
		default:
			break;
		}
	} while (!goMenuOut);
}

// Hittar rätt day, returnerar medeltemp/medelfuktighet
double activities::searchDay(std::string date, std::string enviroment, std::string what)
{
	// Medel temperaturen INNE
	if (enviroment == "In" && what == "temp")
	{

		for (int i = 0; i < sizeVectorAverageIn; i++)
		{
			if (AverageInside[i]->get_a_date() == date) // Kollar bara inomhus.
			{
				return AverageInside[i]->get_averageTemp();
			}
		}

	}

	// Medel temperaturen UTE
	else if (enviroment == "Out" && what == "temp")
	{
		for (int i = 0; i < sizeVectorAverageOut; i++)
		{
			if (AverageOutside[i]->get_a_date() == date) // Kollar bara inomhus.
			{
				return AverageOutside[i]->get_averageTemp();
			}
		}
	}



}


void activities::testAverage()
{
	/* medelvärden inne */
	float changeTemp, changeHum;
	std::string datum;

	datum = inside[0]->get_date(); 	// Hämta medeltemp och medelfukt.
	testCount("In", changeTemp, changeHum, datum); // ändrar changeTemp & changeHum till medelvärden.
	AverageInside.push_back(new AverageDay(datum, changeTemp, changeHum));

	for (int i = 0; i < sizeVectorInside - 2; i++) // kollar hela vectorn.
	{
		// Om de datum brevid varandra INTE är samma, ger unika datum. Varje datum endast EN gång.
		if (inside[i]->get_date() != inside[i + 1]->get_date())
		{

			// Hämta medeltemp och medelfukt.
			datum = inside[i + 1]->get_date();
			testCount("In", changeTemp, changeHum, datum);
			AverageInside.push_back(new AverageDay(datum, changeTemp, changeHum));
		}
	}


	//* medelvärden ute */
	float changeTemp2, changeHum2;
	std::string datum2;

	datum2 = outside[0]->get_date();
	testCount("Out", changeTemp2, changeHum2, datum);
	AverageOutside.push_back(new AverageDay(datum2, changeTemp2, changeHum2));

	for (int i = 0; i < sizeVectorOutside - 2; i++)
	{
		// Om de datum brevid varandra INTE är samma, ger unika datum. Varje datum endast EN gång.
		if (outside[i]->get_date() != outside[i + 1]->get_date())
		{
			datum2 = outside[i]->get_date();
			testCount("Out", changeTemp2, changeHum2, datum);
			AverageOutside.push_back(new AverageDay(datum2, changeTemp2, changeHum2));
		}
	}

	sizeVectorAverageIn = AverageInside.size();
	sizeVectorAverageOut = AverageOutside.size();
}

void activities::testCount(std::string en, float &temp, float &hum, std::string date)
{
	counter = 0, summa = 0, counter2 = 0, summa2 = 0;	// Nollställer räknare

	if (en == "In")
	{ 
		for (int i = 0; i < sizeVectorInside; i++)  // Loopar så många gånger som den hämtat indata till "inomhus".
		{
			if (inside[i]->get_date() == date) // Kollar bara inomhus.
			{
				// medeltemp.
				summa = summa + inside[i]->get_temp();
				counter++; 

				// medelfuktighet.
				summa2 = summa2 + inside[i]->get_moist();
				counter2++; 
			}
		}

		temp = summa / counter;
		hum = summa2 / counter2;

	}

		else if (en == "Out")
		{ 
			for (int i = 0; i < sizeVectorOutside; i++)  // Loopar så många gånger som den hämtat indata till "inomhus".
			{
				if (outside[i]->get_date() == date) // Kollar bara inomhus.
				{
					// medeltemp.
					summa = summa + outside[i]->get_temp();
					counter++;

					// medelfuktighet.
					summa2 = summa2 + outside[i]->get_moist();
					counter2++;
				}
			}

			temp = summa / counter;
			hum = summa2 / counter2;
		}
}

// Printar medeltemp alla dagar.
void activities::printAverage(std::string enviroment)
{
	if (enviroment == "In")
	{
		std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
		for (int i = 0; i < AverageInside.size(); i++)
		{
		
			std::cout << AverageInside[i]->get_a_date() << " Indoor \nAverage temp: "
				<< AverageInside[i]->get_averageTemp() << std::endl;

			std::cout << "Average humidity: " << AverageInside[i]->get_averageMoist() << std::endl;
			std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;

		}
	}

	if (enviroment == "Out")
	{
		std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
		for (int i = 0; i < AverageOutside.size(); i++)
		{

			std::cout << AverageOutside[i]->get_a_date() << " Outdoor \nAverage temp: "
				<< AverageOutside[i]->get_averageTemp() << std::endl;

			std::cout << "Average humidity: " << AverageOutside[i]->get_averageMoist() << std::endl;
			std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;

		}
	}
}

// Metrologisk höst/vinter
void activities::metrologisk()
{

	/*Datum för metrologisk höst*/

	for (int i = 0; i < sizeVectorAverageOut - 4; i++) // så den inte jämför den sista med ingenting.
	{
		// Om månaden är månad 8 eller senare.
		if (AverageOutside[i]->get_a_date()[5] == 0 && AverageOutside[0]->get_a_date()[6] > 7 || AverageOutside[i]->get_a_date()[5] != 0)
		{

			if (AverageOutside[i]->get_averageTemp() < 10 &&
				AverageOutside[i + 1]->get_averageTemp() < 10 &&
				AverageOutside[i + 2]->get_averageTemp() < 10 &&
				AverageOutside[i + 3]->get_averageTemp() < 10 &&
				AverageOutside[i + 4]->get_averageTemp() < 10)
			{

				std::cout << " Meteorological autumn starts : " << AverageOutside[i]->get_a_date() << std::endl;
				i = sizeVectorAverageOut + 5;

			}

		}

	}

	/*Datum för metrologisk vinter*/

	for (int x = 0; x < sizeVectorAverageOut - 4; x++) // så den inte jämför den sista med ingenting.
	{

		// Kollar först alla månader 10-12. 
		if (AverageOutside[x]->get_a_date()[5] != 0)
		{

			if (AverageOutside[x]->get_averageTemp() < 0 &&
				AverageOutside[x + 1]->get_averageTemp() < 0 &&
				AverageOutside[x + 2]->get_averageTemp() < 0 &&
				AverageOutside[x + 3]->get_averageTemp() < 0 &&
				AverageOutside[x + 4]->get_averageTemp() < 0)
			{

				std::cout << " Meteorological winter starts : " << AverageOutside[x]->get_a_date() << std::endl << std::endl;
				x = sizeVectorAverageOut + 5;

			}
		}
		else
			// Kollar sedan månaderna 1-9
			if (AverageOutside[x]->get_a_date()[5] == 0)
			{

				if (AverageOutside[x]->get_averageTemp() < 0 &&
					AverageOutside[x + 1]->get_averageTemp() < 0 &&
					AverageOutside[x + 2]->get_averageTemp() < 0 &&
					AverageOutside[x + 3]->get_averageTemp() < 0 &&
					AverageOutside[x + 4]->get_averageTemp() < 0)
				{

					std::cout << " Meteorological winter starts : " << AverageOutside[x]->get_a_date() << std::endl << std::endl;
					x = sizeVectorAverageOut + 5;

				}

			}

	}
	

}

// Mögelrisk.
void activities::mould()
{

	double fuktighet = 0, temp, mouldLine, mouldRisk;

	for (int i = 0; i < sizeVectorAverageOut; i++)
	{
		// Hämtar in medeltemp.
		temp = AverageOutside[i]->get_averageTemp();

		// Hittar gränser för mögelrisk.
		mouldLine = ((-0.0015 * temp) * (-0.0015 * temp) * (-0.0015 * temp))
			+ ((0.1193 * temp) * (0.1193 * temp))
			- (2.9878 * temp) + 102.96;

		// hämtar in fuktighet.
		fuktighet = AverageOutside[i]->get_averageMoist();

		// mögelrisken blir
		mouldRisk = fuktighet - mouldLine;

		AverageOutside[i]->set_mouldRisk(mouldRisk);
	
	}

	for (int i = 0; i < sizeVectorAverageIn; i++)
	{
		// Hämtar in medeltemp.
		temp = AverageInside[i]->get_averageTemp();

		// Hittar gränser för mögelrisk.
		mouldLine = ((-0.0015 * temp) * (-0.0015 * temp) * (-0.0015 * temp))
			+ ((0.1193 * temp) * (0.1193 * temp))
			- (2.9878 * temp) + 102.96;

		// hämtar in fuktighet.
		fuktighet = AverageOutside[i]->get_averageMoist();

		// mögelrisken blir
		mouldRisk = fuktighet - mouldLine;

		AverageInside[i]->set_mouldRisk(mouldRisk);

	}

}

void activities::printMould()
{

	std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
	for (int i = 0; i < sizeVectorAverageOut; i++)
	{
		std::cout << "Mouldrisk: " << AverageOutside[i]->get_mouldRisk() << std::endl;
		std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
	}

	std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
	for (int i = 0; i < sizeVectorAverageIn; i++)
	{
		std::cout << "Mouldrisk: " << AverageInside[i]->get_mouldRisk() << std::endl;
		std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
	}


}

