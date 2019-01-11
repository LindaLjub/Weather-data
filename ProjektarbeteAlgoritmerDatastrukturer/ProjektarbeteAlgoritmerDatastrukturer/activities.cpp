#include "pch.h"
#include "activities.h"
#include <fstream>
#include <string>

// Konstruktor. H�mtar in all data till vektorerna EN g�ng! N�r programmet startar.
activities::activities()
{
	indata();
	average();
	mould();
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
		while (!datafile.eof())
		{

			getline(datafile, date, ' ');
			getline(datafile, time, ',');
			getline(datafile, place, ',');
			getline(datafile, temp, ',');
			getline(datafile, moist);

			// Kollar om det �r inne eller ute. Skickar in datan i vektorn.
			if (place == "Inne")
			{
				inside.push_back(new theData(date, time, place, stof(temp), stof(moist)));

			}
			else
			{
				outside.push_back(new theData(date, time, place, stof(temp), stof(moist)));

			}
		}
		datafile.close(); // St�nger filen n�r man �r f�rdig med den.
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
	std::cout << " MAIN MENU\n [1] Indoor data\n [2] Outdoor data\n [3] Exit \n [4] highest mouldrisk - test" << std::endl;
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
		case '4':
			testSort();
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
			printAverage("In"); // printar medelv�rden inne.
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

	std::string datum;
	int counter = 1;
	float summa = inside[0]->get_temp();
	float summa2 = inside[0]->get_moist();

	for (int i = 0; i < sizeVectorInside - 2; i++) // kollar hela vectorn.
	{
		// om datumen brevid varandra �r samma.
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


	/* medelv�rden ute */

	counter = 1;
	summa = outside[0]->get_temp();
	summa2 = outside[0]->get_moist();

	for (int i = 0; i < sizeVectorOutside - 2; i++) // kollar hela vectorn.
	{
		// om datumen brevid varandra �r samma.
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

				std::cout << "1 Meteorological winter starts : " << Average[x]->get_a_date() << std::endl << std::endl;
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

					std::cout << "2 Meteorological winter starts : " << Average[x]->get_a_date() << std::endl << std::endl;
					x = Average.size() + 5;

				}

			}
		}
	}

}

// M�gelrisk.
void activities::mould()
{

	float fuktighet = 0, temp, mouldLine, mouldRisk;

	for (int i = 0; i < Average.size(); i++)
	{
		// H�mtar in medeltemp.
		temp = Average[i]->get_averageTemp();

		// Hittar gr�nser f�r m�gelrisk.
		mouldLine = ((-0.0015 * temp) * (-0.0015 * temp) * (-0.0015 * temp))
			+ ((0.1193 * temp) * (0.1193 * temp))
			- (2.9878 * temp) + 102.96;

		// h�mtar in fuktighet.
		fuktighet = Average[i]->get_averageMoist();

		// m�gelrisken blir
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

// printar ut m�gelrisk.
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

void activities::testSort()
{
	float array[260];
	int count = 0;

	// array f�r alla v�rden i Average.
	for (int i = 0; i < Average.size(); i++)
	{
		array[i] = Average[i]->get_mouldRisk();
		count++;
	}


	// sortera
	mergeSort(array, 0, 260 - 1);


	// Hitta datum

	std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
	std::cout << " Highest mouldrisk: " << std::endl;
	std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;

	for (int x = 259;  x > 254; x--) // Hittar topp 5
	{

		for (int i = 0; i < Average.size(); i++) // loppar hela arrayen.
		{

			if (array[x] == Average[i]->get_mouldRisk())
			{
				std::cout << " #" << Average[i]->get_a_date();

				if (Average[i]->get_indoor() == true)
				{
					std::cout << " indoor " << std::endl;
				}
				else
				{
					std::cout << " outdoor " << std::endl;
				}
			}
		}
	}

	std::cout << "\n";
		
		
	


}

void activities::merge(float * array, int start, int end)
{

	int middle = (start + end) / 2; // hittar en ny mitten.

	int i = start;
	int j = middle + 1;
	int k = start;

	float temporary[260];// anv�nds som beh�llare f�r arrayens element. // Lika stor, 260 som Average vectorn.

	// j�mf�ra elementen.
	while (i <= middle && j <= end) // S�l�nge som f�rsta elementet �r mindre �n sista elementet i del 1 och samma sak i del 2.
	{
		if (array[i] < array[j]) // om elemnent 1 del 1 �r mindre �n element 1 del 2.
		{
			temporary[k] = array[i]; // beh�llarens plats 1 = element 1 del 1.
			k++;
			i++;
		}
		else
		{
			temporary[k] = array[j]; //  beh�llarens plats 1 = element 1 del 2.
			k++;
			j++;
		}
	}

	// Fattar ej dessa 2..

	while (i <= middle) // s�l�nge som i �r mindre eller lika med mitten.
	{
		temporary[k] = array[i]; // starten p� beh�llaren = element 1 del 2.
		k++;
		i++;
	}

	while (j <= end)
	{
		temporary[k] = array[j];
		k++;
		j++;
	}

	// Kopiera alla element fr�n beh�llaren till den riktiga arrayen.
	for (int i = start; i <= end; i++)
	{
		array[i] = temporary[i];
	}

}

void activities::mergeSort(float array[], int start, int end)
{

	// om det �r 0 eller 1 element beh�vs det inte sorteras.
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

	// steg 3. Merge dom tv� delarna.
	merge(array, start, end); // sortera arrayen fr�n start till slut.

}
