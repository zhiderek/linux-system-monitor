#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid): pid_(pid) {};
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();            // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process a) ;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
	int pid_;
	enum pTimeStates {
		utime_ = 14,
		stime_ =  15,
		cutime_ = 16,
		cstime_ = 17,
		starttime_ = 22
	};
};

#endif
