#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS 
string Format::ElapsedTime(long seconds) { 
	int remainder = 0;
	int hours = 0;
	int min = 0;
	int sec = 0;
	
	hours = seconds / 3600;
	remainder = seconds%3600;
	min = remainder/60;
	sec = remainder%60;
	return std::to_string(hours) + ":" + std::to_string(min) + ":" + std::to_string(sec);
}
