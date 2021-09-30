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

Process::Process(const int pid) : pid_(pid), user_(LinuxParser::User(pid)), command_(LinuxParser::Command(pid)) {};

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float  Process::CpuUtilization() const { return 0; } 

// TODO: Return the command that generated this process
string Process::Command() const { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() const { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// This < that when this uses less CPU than that.
bool Process::operator<(Process const& that) const { 
    return Pid() > that.Pid();
 //   return CpuUtilization() < that.CpuUtilization() ? 0 : 1; 
}