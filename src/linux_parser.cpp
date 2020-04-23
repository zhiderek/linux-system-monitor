#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <exception>
#include "linux_parser.h"
#include <assert.h>
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
// Memory Utilization = 1.0 - (free_memory / (total_memory - buffers))
float LinuxParser::MemoryUtilization() { 
  // int MemTotal, MemFree, buffers;
  string key, size;
  float value;
  string line;
  std::unordered_map<string, float> memoMap;
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
  int stopCounter = 0;
  if (stream.is_open()) {
    while (memoMap.size()<3 && std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value >> size;
      if (key == "MemTotal") {
        memoMap["MemTotal"] = value;
      }
      if (key == "MemFree") {
        memoMap["MemFree"] = value;
      }
      if (key == "Buffers") {
        memoMap["Buffers"] = value;
      }

      if (stopCounter > 100000) {
        throw std::runtime_error("wrong input file?");
      }
      stopCounter++;
    }
	assert (memoMap.size() ==3);

    if (memoMap.size()==3) {
      return 1.0 - (memoMap["MemFree"] / (memoMap["MemTotal"] - memoMap["Buffers"]));
    }
  }
  return -1.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
	long UpTime = -1;
	string line;
	std::ifstream stream(kProcDirectory+kUptimeFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> UpTime;
	}
	return UpTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
	long totalJiffies =  ActiveJiffies() + IdleJiffies();
	return totalJiffies;
}

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
	long jiffi=0;
	long activeJiffiSum = 0;
	string line;
	std::ifstream stream(kProcDirectory+"/"+ std::to_string(pid) + "/" + kStatFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		for (int i=0; i<13; i++) {
			linestream >> jiffi;
		}
		for (int i=0; i<4; i++) {
			linestream >> jiffi;
			activeJiffiSum += jiffi;
		}
	}
	return activeJiffiSum;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
	vector<CPUStates> activeStates = {
		kUser_,
		kNice_,
		kSystem_,
		kIRQ_,
		kSoftIRQ_,
		kSteal_
	};
	long activeJiffies = 0;
    vector<string> utilization = CpuUtilization();
    for (CPUStates state: activeStates) {
         activeJiffies += stol(utilization[state]);
	}
    return activeJiffies;
}


// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
	vector<CPUStates> idleStates = {
		kIdle_,
		kIOwait_
	};
	long idleJiffies = 0; 
	vector<string> utilization = CpuUtilization();
	for (CPUStates state: idleStates) {
		 idleJiffies += stol(utilization[state]);
	}
	return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
	vector<string> utilization; //a vector of strings containing the kinds of work
	string line, token;
	std::ifstream stream(kProcDirectory+kStatFilename);
	if (stream.is_open()) {
		getline(stream, line);
		std::istringstream linestream(line);
		while (linestream >> token) {
			if (token == "cpu") {
				continue;
			}
			utilization.emplace_back(token);
		}
		assert(utilization.size() == 10);
	}
	return utilization;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
	string key;
	int totalProcesses;
	string line;
	std::ifstream stream(kProcDirectory+kStatFilename);
	if (stream.is_open()) {
		while (std::getline(stream, line)) {
			//parse the first token to check if it's processes
			std::istringstream linestream(line);
			linestream >> key;
			if (key == "processes") {
				linestream >> totalProcesses;
				return totalProcesses;
			}
		}
	}
	return -1;
}
// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
	string key;
    int runningProcesses;
    string line;
    std::ifstream stream(kProcDirectory+kStatFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            //parser the first token to check if it's processes
            std::istringstream linestream(line);
            linestream >> key;
            if (key == "procs_running") {
                linestream >> runningProcesses;
                return runningProcesses;
            }
        }
    }
    return -1;
}


// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
	string token, line;
	string command = "";
    std::ifstream stream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		while (linestream >> token) {
			command += token;
		}
	}
	return command;
}


// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
	string key, value;
    string line;
    std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatusFilename);
    if (stream.is_open()) {
        while(std::getline(stream, line)){
			std::replace(line.begin(), line.end(), ':', ' ');
    	    //parser the first token to check if it's processes
       	    std::istringstream linestream(line);
        	linestream >> key;
			if (key == "VmSize") {
				linestream >> value;
				int vmSize = std::stoi(value);
				vmSize = vmSize / 1024;
				return std::to_string(vmSize);
			}
		}
    }
	return string();	
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
	string token, line, uid;
	std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatusFilename);
	if (stream.is_open()) {
		while (std::getline(stream, line)) {
			std::replace(line.begin(), line.end(), ':', ' ');
			std::istringstream linestream(line);
			linestream >> token;
			if (token == "Uid") {
				linestream >> uid;
				return uid;
			}
		}
	}
	return string();
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
	string uid = Uid(pid);
	string token, line, user, x, uid_;
	std::ifstream stream(kPasswordPath);
	if (stream.is_open()) {
		while (std::getline(stream, line) ) {
			std::replace(line.begin(), line.end(), ':', ' ');
			std::istringstream linestream(line);
			if (linestream >> user >> x >> uid_) {
				if (uid_ == uid) {
					return user;
				}
			}
		}
	}
	return string();
}		
// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
	string jiffi;
    string line;
    std::ifstream stream(kProcDirectory+"/"+ std::to_string(pid) + "/" + kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
		std::replace(line.begin(), line.end(), '(', ' ');
		std::replace(line.begin(), line.end(), ')', ' ');		
		std::replace(line.begin(), line.end(), '-', '_');
		std::istringstream linestream(line);
        for (int i=0; i<22; i++) {
            linestream >> jiffi;
        }
	}
    return stol(jiffi)/sysconf(_SC_CLK_TCK);	
}
