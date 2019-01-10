#include "pch.h"
#include "activities.h"
#include <fstream>
#include <string>

// Konstruktor. Hämtar in all data till vektorerna EN gång! När programmet startar.
activities::activities()
{
	indata();
	average();
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
	bool goMenuIn = false;
	do {
		std::cout << " INDOOR MENU\n [1] Search for average temp by date \n [2] Daily average temp & humidity \n [3] Go back" << std::endl;
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
			printAverage("In"); // printar medelvärden inne.
			break;
		case '3':
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
			searchDay(searchDate, "Out");	
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

	std::string datum;
	int counter = 1;
	float summa = inside[0]->get_temp();
	float summa2 = inside[0]->get_moist();

	for (int i = 0; i < sizeVectorInside - 2; i++) // kollar hela vectorn.
	{
		// om datumen brevid varandra är samma.
		if (inside[i]->get_date() == inside[i + 1]->get_date())
		{
			// medeltemp.
			summa = summa + inside[i + 1]->get_temp();
				
			// medelfuktighet.
			summa2 = summa2 + inside[i]->get_moist();
			counter++;
		}

		else 
		{
			datum = inside[i]->get_date();
			Average.push_back(new AverageAll(datum, (summa / counter), (summa2 / counter), true)); // test test

			summa = 0, summa2 = 0;

			summa = inside[i + 1]->get_temp();
			summa2 = inside[i + 1]->get_moist();

			counter = 1;

		}
	}


	/* medelvärden ute */

	counter = 1;
	summa = outside[0]->get_temp();
	summa2 = outside[0]->get_moist();

	for (int i = 0; i < sizeVectorOutside - 2; i++) // kollar hela vectorn.
	{
		// om datumen brevid varandra är samma.
		if (outside[i]->get_date() == outside[i + 1]->get_date())
		{
			// medeltemp.
			summa = summa + outside[i + 1]->get_temp();

			// medelfuktighet.
			summa2 = summa2 + outside[i]->get_moist();
			counter++;
		}

		else
		{
			datum = outside[i]->get_date();
			Average.push_back(new AverageAll(datum, (summa / counter), (summa2 / counter), false)); 

			summa = 0, summa2 = 0;

			summa = outside[i + 1]->get_temp();
			summa2 = outside[i + 1]->get_moist();

			counter = 1;

		}
	}


	//sizeVectorAverageIn = AverageInside.size();
	//sizeVectorAverageOut = AverageOutside.size();



}

// Printar medeltemp alla dagar.
void activities::printAverage(std::string enviroment)
{
	std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;

	for (int i = 0; i < Average.size(); i++)
	{

		if (enviroment == "In" && Average[i]->get_indoor() == true)
		{
			std::cout << " Indoor ";
			std::cout << Average[i]->get_a_date() << "\n Average temp: "
				<< Average[i]->get_averageTemp() << std::endl;
			std::cout << " Average humidity: " << Average[i]->get_averageMoist() << std::endl;
			std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;

		}

		else if (enviroment == "Out" && Average[i]->get_indoor() == false)
		{
			std::cout << " Outdoor " ;
			std::cout << Average[i]->get_a_date() << "\n Average temp: "
			<< Average[i]->get_averageTemp() << std::endl;
			std::cout << " Average humidity: " << Average[i]->get_averageMoist() << std::endl;
			std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;

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
				Average[i]->get_indoor() == false && Average[i + 1]->get_averageTemp() < 10 &&
				Average[i + 2]->get_indoor() == false && Average[i + 2]->get_averageTemp() < 10 &&
				Average[i + 3]->get_indoor() == false && Average[i + 3]->get_averageTemp() < 10 &&
				Average[i + 4]->get_indoor() == false && Average[i + 4]->get_averageTemp() < 10)
			{

				std::cout << " Meteorological autumn starts : " << Average[i]->get_a_date() << "\n" << std::endl;
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

				std::cout << "1 Meteorological winter starts : " << Average[x]->get_a_date() << std::endl << std::endl;
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

					std::cout << "2 Meteorological winter starts : " << Average[x]->get_a_date() << std::endl << std::endl;
					x = Average.size() + 5;

				}

			}
		}
	}

}

// Mögelrisk.
void activities::mould()
{

	float fuktighet = 0, temp, mouldLine, mouldRisk;

	for (int i = 0; i < Average.size(); i++)
	{
		// Hämtar in medeltemp.
		temp = Average[i]->get_averageTemp();

		// Hittar gränser för mögelrisk.
		mouldLine = ((-0.0015 * temp) * (-0.0015 * temp) * (-0.0015 * temp))
			+ ((0.1193 * temp) * (0.1193 * temp))
			- (2.9878 * temp) + 102.96;

		// hämtar in fuktighet.
		fuktighet = Average[i]->get_averageMoist();

		// mögelrisken blir
		mouldRisk = fuktighet - mouldLine;

		Average[i]->set_mouldRisk(mouldRisk);

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
void activities::printMould()
{
	std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
	for (int i = 0; i < Average.size(); i++)
	{
		if (Average[i]->get_indoor() == true)
		{
			std::cout << " Indoor ";
		}
		else
		{
			std::cout << " Outdoor ";
		}

		std::cout << Average[i]->get_a_date() << std::endl;
		std::cout << " Mouldrisk: " << Average[i]->get_mouldBool() << std::endl;
		std::cout << " " << Average[i]->get_mouldRisk() << std::endl;
		std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
	}
}

