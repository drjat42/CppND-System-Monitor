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
  int Pid() const;                          // TODO: See src/process.cpp
  std::string User() const;            // TODO: See src/process.cpp
  std::string Command() const;            // TODO: See src/process.cpp
  float CpuUtilization() const; //maybe?            // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& that) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
   int pid_; //  WTF, this can't be const and sortable?!
   std::string  user_;
   std::string  command_;

};

#endif