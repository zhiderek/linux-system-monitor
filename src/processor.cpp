#include "processor.h"
#include "linux_parser.h"
#include <iostream>
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
	int prevActive = curtActive;
	int prevTotal = curtTotal;
	curtActive = LinuxParser::ActiveJiffies();
	curtTotal = LinuxParser::Jiffies();
	float res = (curtActive - prevActive) / (curtTotal - prevTotal + epsilon);
	return res;	
}
