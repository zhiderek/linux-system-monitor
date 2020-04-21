#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: XX days, HH:MM:SS //this is same representation of time in htop which makes more sense to me.
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
	int remainder = 0;
	int days = 0;
	int hours = 0;
	int min = 0;
	int sec = 0;
	
	days = seconds/(3600 * 24);
	hours = (seconds - days * (3600 * 24)) / 3600;
	remainder = seconds%3600;
	min = remainder/60;
	sec = remainder%60;
	return std::to_string(days) + " days, " + std::to_string(hours) + ":" + std::to_string(min) + ":" + std::to_string(sec);
}
