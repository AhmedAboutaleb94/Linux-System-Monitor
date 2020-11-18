#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
using namespace std;

class System {
 public:
  Processor& Cpu();              
  vector<Process>& Processes();  
  float MemoryUtilization();      
  long int UpTime();              
  int TotalProcesses();           
  int RunningProcesses();         
  string Kernel();               
  string OperatingSystem();      

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  // Member function holding the processes found to pass to the system.cpp to display
  std::vector<Process> foundProcesses = {};
};

#endif