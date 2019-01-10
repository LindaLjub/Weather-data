#pragma once
class AverageDay
{

private:
	std::string a_date, mouldYes;
	float averageTemp, averageHumidity, mouldRisk;

public:
	AverageDay();
	AverageDay(std::string a_date, float averageTemp, float averageHumidity)
		: a_date(a_date), averageTemp(averageTemp), averageHumidity(averageHumidity) {};


	std::string get_a_date() { return this->a_date; }

	float get_averageMoist() { return this->averageHumidity; }
	float get_averageTemp() { return this->averageTemp; }
	float get_mouldRisk() { return this->mouldRisk; }
	std::string get_mouldBool() { return this->mouldYes; }

	void set_mouldRisk(float a) { mouldRisk = a; }
	void set_mouldBool(std::string a) { mouldYes = a; }

	~AverageDay(); 
};

