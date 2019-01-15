#pragma once
class AverageAll
{

private:
	std::string a_date, mouldYes;
	float averageTemp, averageHumidity, mouldRisk, tempDiff;
	bool indoor;

public:
	AverageAll();
	AverageAll(std::string a_date, float averageTemp, float averageHumidity, bool indoor)
		: a_date(a_date), averageTemp(averageTemp), averageHumidity(averageHumidity), indoor(indoor) {};

	// getters
	std::string get_a_date() { return this->a_date; }
	bool get_indoor() { return this->indoor;  }
	float get_averageMoist() const { return this->averageHumidity; }
	float get_averageTemp() const { return this->averageTemp; } 

	float get_mouldRisk() const { return this-> mouldRisk; }
	std::string get_mouldBool() { return this-> mouldYes; }
	float get_diff() { return this->tempDiff; }

	// setters
	void set_mouldRisk(float a) { mouldRisk = a; }
	void set_tempDiff(float a) { tempDiff = a; }
	void set_mouldBool(std::string a) { mouldYes = a; }

	~AverageAll();
};

