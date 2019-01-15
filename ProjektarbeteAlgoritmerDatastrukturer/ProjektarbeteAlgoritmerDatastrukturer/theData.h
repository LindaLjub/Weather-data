#pragma once
class theData
{

private:
	// alla variabler
	std::string date, time, place;
	float temp, moist;
	bool doorClosed;
	
public:
	// konstrktor, en utan indata och en med. konstruktorn initierar variablerna.
	theData() {};
	theData(std::string date, std::string time, float temp, std::string place) : date(date), time(time), temp(temp), place(place) {};
	theData(std::string date, std::string time, std::string place, float temp, float moist)
		: date(date), time(time), place(place), temp(temp), moist(moist) {};

	// getters
	std::string get_date() { return this->date; }
	std::string get_time() { return this->time; }
	std::string get_place() { return this->place; }
	float get_temp() { return this->temp; }
	float get_moist() { return this->moist; }
	bool get_doorClosed() { return this->doorClosed; }

	// setters
	void set_d_tempe(float a) { this->temp = a; }
	void set_d_moist(float a) { this->moist = a; } 
	void set_doorClosed(bool a) { this->doorClosed = a; }

	~theData();
};

