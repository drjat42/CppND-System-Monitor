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
  std::string Ram();         
  long int UpTime();                  
  bool operator<(Process const& that) const; 

 private:
   int pid_; //  WTF, this can't be const and sortable?!
   std::string  user_;
   std::string  command_;
};

#endif