#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include "linux_parser.h"

using namespace std;

class Process {
 public:
  Process(int pid);
  int Pid();
  string User();                      
  string Command();                   
  float CpuUtilization();                  
  string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

 private:
    // A private member to hold the PID of the process
    int pid_ ;
};

#endif