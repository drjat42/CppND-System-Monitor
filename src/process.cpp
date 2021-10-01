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

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float  Process::CpuUtilization()  { 
    long activeJiffies = LinuxParser::ActiveJiffies(Pid());   
    long activeTime = activeJiffies / sysconf(_SC_CLK_TCK); // Convert from clock ticks to seconds.
    long uptime = UpTime();
    float utilization = uptime == 0 ? 0 : activeTime / uptime;
    return utilization;
 } 

// Return the command that generated this process
string Process::Command() const { return command_; }

// Return this process's memory utilization
string Process::Ram() { 
    long kb = std::stol(LinuxParser::Ram(Pid()));
    string mb = to_string(std::round((kb / 1000)));  // Convert to whole megabytes
    return mb.substr(0, mb.find('.'));               // Lop off the decimal zeros
}

// Return the user (name) that generated this process
string Process::User() const { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// This < that when this uses less CPU than that.
bool Process::operator<(Process const& that) const { 
    return Pid() > that.Pid();
 //   return CpuUtilization() < that.CpuUtilization() ? 0 : 1; 
}