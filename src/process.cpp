#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;


// TODO: Return this process's ID
int Process::Pid() {
	return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
	vector<string> times;
	string line, token;
	std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(Pid()) + LinuxParser::kStatFilename);
	if (! stream.is_open()) {
		return -1;
	}
	std::getline(stream, line);
	std::istringstream linestream(line);
	while (linestream >> token) {
		times.emplace_back(token);
	}
	long uptime = LinuxParser::UpTime();
	long utime = std::stol(times[utime_]);
	long stime = std::stol(times[stime_]);
	long cutime = std::stol(times[cutime_]);
	long cstime = std::stol(times[cstime_]);
	long starttime = std::stol(times[starttime_]);
	//copied formulae from https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
	long total_time = utime + stime + cutime + cstime;
	long seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
	float cpu_usage = (1.0 * total_time / sysconf(_SC_CLK_TCK)) / seconds;
	return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() {
	return LinuxParser::Command(pid_);
 }

// TODO: Return this process's memory utilization
string Process::Ram() {
	return LinuxParser::Ram(pid_);
 }

// TODO: Return the user (name) that generated this process
string Process::User() {
	return LinuxParser::User(pid_);
 }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
	return LinuxParser::UpTime(pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator < (Process a) { 
	return this->CpuUtilization() > a.CpuUtilization();	
}
