#pragma once
#include <iostream>
class theData
{

private:
	// alla variabler
	std::string date, time, place;
	float temp, moist;
	std::string doorClosed, closedDate, closedTime;
	int timeOpen;
	bool OpenNr2;
	
public:
	// konstruktors.
	theData() {};
	theData(std::string date, std::string time, std::string place, float temp, float moist) // till inne7ute vektorerna.
		: date(date), time(time), place(place), temp(temp), moist(moist) {};

	theData(std::string date, float temp, float moist, std::string time) : date(date), temp(temp), moist(moist), time(time) {}; // till balkongdörr.
	theData(std::string date, std::string time, int timeOpen, std::string closedDate, std::string closedTime): 
		date(date), time(time), timeOpen(timeOpen), closedDate(closedDate), closedTime(closedTime) {}; // till balkongdörr.

	// getters
	std::string get_date() { return this->date; }
	std::string get_time() { return this->time; }
	std::string get_place() { return this->place; }

	float get_temp() { return this->temp; }
	float get_moist() { return this->moist; }
	std::string get_doorClosed() { return this->doorClosed; } 
	int get_hoursOpened() const { return this->timeOpen; }
	std::string get_ClosedDate() { return this->closedDate; }
	std::string get_ClosedTime() { return this->closedTime; }
	bool get_OpenNr2() { return this->OpenNr2; }

	// setters
	void set_d_tempe(float a) { this->temp = a; }
	void set_d_moist(float a) { this->moist = a; } 
	void set_doorClosed(std::string a) { this-> doorClosed = a; }
	void set_OpenNr2(bool a) { this->OpenNr2; }


	~theData();
};
