#include "pch.h"
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
	//doorOpen();
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


// Sortera efter medeltemperatur
bool hej(const AverageAll *a, const AverageAll *b)
{
	return a->get_averageTemp() < b->get_averageTemp();
};

// Sortera efter medelfuktighet
bool sortHum(const AverageAll *a, const AverageAll *b)
{
	return a->get_averageMoist() < b->get_averageMoist();
};

// sortera efter mögelrisk
bool sortMould(const AverageAll *a, const AverageAll *b)
{
	return a->get_mouldRisk() < b->get_mouldRisk();
};


// Huvudmenyn.
void activities::menuOne()
{
	bool goMenu = false;
	do {
	std::cout << " MAIN MENU\n [1] Indoor data\n [2] Outdoor data\n [4] Temp diff \n [5] Door open?\n [6] Exit" << std::endl;
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
			diff();
			break;
		case '5':
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

	std::string datum, tid;
	int counter = 1, test1 = 0;
	float summa = inside[0]->get_temp();
	float summa2 = inside[0]->get_moist();

	int testdoor = 0;

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

			if (testdoor < 1) // vector som jämför dagar.
			{
				datum = inside[i]->get_date();
				tid = inside[i]->get_time();
				door.push_back(new theData(datum, tid, inside[i]->get_temp(), "Inne"));
			}
			

		}

		else 
		{
		
			datum = inside[i]->get_date();
			Average.push_back(new AverageAll(datum, (summa / counter), (summa2 / counter), true)); // test test

			// till door
			if (testdoor < 1) // vector som jämför dagar.
			{
				tid = inside[i]->get_time();
				door.push_back(new theData(datum, tid, inside[i]->get_temp(), "Inne"));
			}

			testdoor++; // till door

			summa = 0, summa2 = 0;

			summa = inside[i + 1]->get_temp();
			summa2 = inside[i + 1]->get_moist();

			counter = 1;
			test1++;


		}
	}

	datum = inside[test1]->get_date();
	Average.push_back(new AverageAll(datum, (summa / counter), (summa2 / counter), true)); 


	/* medelvärden ute */
	int test = 0;
	counter = 1;
	summa = outside[0]->get_temp();
	summa2 = outside[0]->get_moist();

	testdoor = 0;

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

			if (testdoor < 1) // vector som jämför dagar.
			{
				datum = outside[i]->get_date();
				tid = outside[i]->get_time();
				door.push_back(new theData(datum, tid, outside[i]->get_temp(), "Ute"));

			}
		}

		else
		{
			test++;

			datum = outside[i]->get_date();
			Average.push_back(new AverageAll(datum, (summa / counter), (summa2 / counter), false)); 

			// till door
			if (testdoor < 1) // vector som jämför dagar.
			{
				tid = outside[i]->get_time();
				door.push_back(new theData(datum, tid, outside[i]->get_temp(), "Ute"));
			}
			testdoor++; // till door


			summa = 0, summa2 = 0;

			summa = outside[i + 1]->get_temp();
			summa2 = outside[i + 1]->get_moist();

			counter = 1;
		}

	}

	// Sista dagen
	datum = outside[test]->get_date();
	Average.push_back(new AverageAll(datum, (summa / counter), (summa2 / counter), false));

	std::cout << " storlek på door vector " << door.size() << std::endl;

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

void activities::merge(float * array, int start, int end)
{

	int middle = (start + end) / 2; // hittar en ny mitten.

	int i = start;
	int j = middle + 1;
	int k = start;

	float temporary[262];// används som behållare för arrayens element. // Lika stor, 260 som Average vectorn.

	// jämföra elementen.
	while (i <= middle && j <= end) // Sålänge som första elementet är mindre än sista elementet i del 1 och samma sak i del 2.
	{
		if (array[i] < array[j]) // om elemnent 1 del 1 är mindre än element 1 del 2.
		{
			temporary[k++] = array[i++]; // behållarens plats 1 = element 1 del 1.
	
		}
		else
		{
			temporary[k++] = array[j++]; //  behållarens plats 1 = element 1 del 2.

		}
	}

	// kommentera dessa

	while (i <= middle) // sålänge som i är mindre eller lika med mitten.
	{
		temporary[k++] = array[i++]; // starten på behållaren = element 1 del 2.

	}

	while (j <= end)
	{
		temporary[k++] = array[j++];
	
	}

	// Kopiera alla element från behållaren till den riktiga arrayen.
	for (int i = start; i <= end; i++)
	{
		array[i] = temporary[i];
	}

}

void activities::mergeSort(float array[], int start, int end)
{

	// om det är 0 eller 1 element behövs det inte sorteras.
	if (start >= end)
	{
		return;
	}

	// 3 steg.

	// steg 1. Dela
	int middle = (start + end) / 2;

	// Steg 2. sortera de 2 delarna. del 1 - start till mitten. del 2 - mitten + 1 till slutet.
	mergeSort(array, start, middle); // del 1
	mergeSort(array, middle + 1, end); // del 2

	// steg 3. Merge dom två delarna.
	merge(array, start, end); // sortera arrayen från start till slut.

}

// räknar ut diffen, setter diff.
void activities::diff()
{
	// Sortering på då inne - och yttertemperaturen skiljt sig mest och minst.
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
			}
			i++;
		}
		x++;
		i = x + 1;
	}

	std::cout << " Diff: ";
	for (int i = 0; i < 10; i++)
	{

		std::cout << " " << Average[i]->get_diff() << " " << Average[i]->get_a_date();
	}

}

// kollar om balkongdörren är öppen
void activities::doorOpen() {


	
	//float diffTemp, diffHum;

	//for (int x = 0; x < 50; x++) // kollar alla innevärden.
	//{
	//	for (int i = 0; i < outside.size(); i++) // mot alla utevärden.
	//	{
	//		if (inside[0]->get_date() == outside[i]->get_date()) // om det är samma datum
	//		{
	//			if (inside[x]->get_time()[0] == outside[i]->get_time()[0] && inside[x]->get_time()[1] == outside[i]->get_time()[1]) // om det är samma timme
	//			{
	//				// jämför temp och fuktighet
	//				diffTemp = inside[x]->get_temp() - outside[i]->get_temp();
	//				diffHum = inside[x]->get_moist() - outside[i]->get_moist();

	//				if (diffTemp < -3 || diffTemp > 3) // om diff är väldigt hög/väldigt låg, dörren är inte öppen.
	//				{
	//					inside[x]->set_doorClosed(true);
	//				}

	//				else // Om det är liten diff mellan temp, kolla fuktighet.
	//				{
	//					if (diffHum < 3 || diffHum > 3) // mycket diff, inte öppet.
	//					{
	//						inside[x]->set_doorClosed(true);
	//					}
	//					else // lite diff, dörren är öppen.
	//					{
	//						inside[x]->set_doorClosed(false);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

// visar om balkongdörren är öppen
void activities::printDoor()
{
	//int countTime = 0;
	//int x = 0;



	//for (int i = 0; i < inside.size(); i++)
	//{
	//	countTime = 0;
	//	x = 0;

	//	if (inside[i]->get_doorClosed() == false) // om dörren är öppen
	//	{
	//		std::cout << " open from: " << inside[i]->get_date() <<
	//			" " << inside[i]->get_time() << std::endl;
	//		countTime = 1;
	//		x = i + 1; // nästa temp efter i.

	//			while (inside[x]->get_doorClosed() == false) // så länge som nästa timme är öppen
	//			{
	//				x++;
	//				countTime++;
	//				std::cout << " open to: " << inside[x]->get_date() << " " << inside[x]->get_time() << std::endl;
	//			}

	//			i = i + countTime;// den kollar inte samma igen.
	//	
	//			
	//	}
	//}


	for (int i = 0; i < door.size(); i++)
	{

	//	std::cout << inside[i]->get_date();
	//		
	//	if (inside[i]->get_doorClosed() == false)
	//	{
	//		std::cout << " open" << std::endl;
	//	}
	//	else if (inside[i]->get_doorClosed() == true)
	//	{
	//		std::cout << " closed" << std::endl;
	//	}
	//		
		std::cout << door[i]->get_date() << " " << door[i]->get_time() << " " << door[i]->get_temp() << " " << door[i]->get_place() << std::endl;
	}
}