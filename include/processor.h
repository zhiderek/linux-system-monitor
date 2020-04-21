#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  //calculate the ratio between difference in active time and difference in total time 

 private:
	long curtActive=0;
	long curtTotal=0;
	float epsilon=0.0001; //avoid floating number overflowing
};

#endif
