#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int const pid);
  int Pid() const;
  std::string User() const; 
  std::string Command() const; 
  float CpuUtilization();  
  std::string Ram() const;     
  long int UpTime() const;            
  bool operator<(Process const& that) const; 

 private:
   int pid_;
   std::string  user_;
   std::string  command_;
   long prevSysJiffies_;
   long prevActiveProcJiffies_;
   float cpuUtilization_;
};

#endif