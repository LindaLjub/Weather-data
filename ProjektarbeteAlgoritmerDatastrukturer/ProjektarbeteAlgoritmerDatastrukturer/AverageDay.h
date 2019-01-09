#pragma once
class AverageDay
{

private:
	std::string a_date;
	double averageTemp, averageHumidity, mouldRisk;

public:
	AverageDay();
	AverageDay(std::string a_date, double averageTemp, double averageHumidity)
		: a_date(a_date), averageTemp(averageTemp), averageHumidity(averageHumidity) {};


	std::string get_a_date() { return this->a_date; }

	double get_averageMoist() { return this->averageHumidity; }
	double get_averageTemp() { return this->averageTemp; }
	double get_mouldRisk() { return this->mouldRisk; }

	void set_mouldRisk(double a) { mouldRisk = a; }

	~AverageDay(); 
};

