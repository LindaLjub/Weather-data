#pragma once
class theData
{

private:
	// alla variabler
	std::string date, time, place;
	float temp, moist;
	
public:
	// konstrktor, en utan indata och en med. konstruktorn initierar variablerna.
	theData() {};
	theData(std::string date, std::string time, std::string place, float temp, float moist)
		: date(date), time(time), place(place), temp(temp), moist(moist) {};

	// getters
	std::string get_date() { return this->date; }
	std::string get_time() { return this->time; }
	std::string get_place() { return this->place; }
	float get_temp() { return this->temp; }
	float get_moist() { return this->moist; }

	// setters
	void set_d_tempe(float a) { this->temp = a; }
	void set_d_moist(float a) { this->moist = a; } 


	~theData();
};

