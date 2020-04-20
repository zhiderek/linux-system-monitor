#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
	int remainder = 0;
	int hour = 0;
	int min = 0;
	int sec = 0;
	
	hour = seconds/3600;
	remainder = seconds%3600;
	min = remainder/60;
	sec = remainder%60;
	return std::to_string(hour) + ":" + std::to_string(min) + ":" + std::to_string(sec);
}
