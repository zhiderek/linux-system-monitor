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
	return LinuxParser::CpuUtilization(pid_); 
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
	return LinuxParser::Uptime(pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator < (Process const& a) const { 
	this->CpuUtilization() > a.CpuUtilization();	
}
