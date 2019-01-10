#pragma once
class AverageAll
{



private:
	std::string a_date;
	float averageTemp, averageHumidity, mouldRisk;
	bool indoor;

public:
	
	AverageAll(std::string a_date, float averageTemp, float averageHumidity, bool indoor)
		: a_date(a_date), averageTemp(averageTemp), averageHumidity(averageHumidity), indoor(indoor) {};


	std::string get_a_date() { return this->a_date; }
	bool get_indoor() { return this->indoor;  }

	float get_averageMoist() { return this->averageHumidity; }
	float get_averageTemp() { return this->averageTemp; }
	float get_mouldRisk() { return this->mouldRisk; }


	AverageAll();
	~AverageAll();
};

