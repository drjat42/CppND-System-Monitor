#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include <linux_parser.h>

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

/* Return the fraction of memeory in use. */
float LinuxParser::MemoryUtilization() {
  float value, total, free;
  string line, key;

  // Total used memory = MemTotal - MemFree 
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
       while (linestream >> key >> value) {
          if (key == "MemTotal:") {
            total = value;
          }
         if (key == "MemFree:") {
            free = value;
          }
        }
    }
  }

  float usage = (total - free)  / total;
  return usage;
}

/* Return the system uptime in seconds */
long LinuxParser::UpTime() { 
  float uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
 }

/* Return the total number of jiffies for the system */
long LinuxParser::Jiffies() { 
  return ActiveJiffies() + IdleJiffies();
 }

/* Return the number of active jiffies for a PID */
long LinuxParser::ActiveJiffies(int pid) { 
  long utime, stime, cutime, cstime;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/" + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    vector<string> items = Split(line, ' ');
    utime = std::stol(items[13]);
    stime = std::stol(items[14]);
    cutime = std::stol(items[15]);
    cstime = std::stol(items[16]);
  }
  return utime + stime + cutime + cstime;
}

/* Return the number of active jiffies for the system */
long LinuxParser::ActiveJiffies() {
  vector<string> jiffs = CpuUtilization();
  return std::stol(jiffs[2]) + std::stol(jiffs[3]) + std::stol(jiffs[4]) + std::stol(jiffs[7]) 
       + std::stol(jiffs[8]) + std::stol(jiffs[9]) + std::stol(jiffs[10]) + std::stol(jiffs[11]);
}

/* Return the number of idle jiffies for the system */
long LinuxParser::IdleJiffies() { 
  vector<string> jiffs = CpuUtilization();
  return std::stol(jiffs[5]) + std::stol(jiffs[6]);
}

/*
 Return a vector of CPU utilization jiffy counts for the system.
 Positional entries in returned vector: 
   [0]"cpu" [1]"" [2]user [3]nice [4]system [5]idle  [6]iowait  [7]irq [8]softirq [9]steal [10]guest [11]nice_guest 
*/
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> utilizations;
  string line, key, values;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
       if (linestream >> key) {
          if (key == "cpu") {
            std::getline(linestream, values);
            utilizations = Split(line, ' ');
          }
        }
    }
  }
  return utilizations;
 }

/* Return the total number of processes */
int LinuxParser::TotalProcesses() { 
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
       while (linestream >> key >> value) {
          if (key == "processes") {
            return value;
          }
        }
    }
  }
  // Should not happen.
  return -42;
 }

/* Return the number of running processes */
int LinuxParser::RunningProcesses() { 
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
       while (linestream >> key >> value) {
          if (key == "procs_running") {
            return value;
          }
        }
    }
  }
  // Should not happen.
  return -42;
 }


/* Return the command associated with a process */
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/" + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);  
  }
  return line;
}

/* Return the memory used by a process */
string LinuxParser::Ram(int pid) { 
   string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/" + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
       while (linestream >> key >> value) {
          if (key == "VmSize:") {
            return value;
          }
        }
    }
  }
  // Should not happen.
  return "-42";
}

/* Return the user ID associated with a process */
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/" + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
       while (linestream >> key >> value) {
          if (key == "Uid:") {
            return value;
          }
        }
    }
  }
  // Should not happen.
  return "-42";
}

/* Return the user associated with a process */
string LinuxParser::User(int pid) { 
  string line;
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      vector<string> items = Split(line, ':');
      std::istringstream linestream(line);
        if (items[2] == uid) {
          return items[0];
        }
      }
    }
  // Should not happen.
  return "-42";
}

/* Return the uptime of a process */
long LinuxParser::UpTime(int pid) {
  long starttime;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/" + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    vector<string> items = Split(line, ' ');
    starttime = std::stol(items[21]);
  }
  starttime /= sysconf(_SC_CLK_TCK); // Convert from clock ticks to seconds.
  long uptime = UpTime() - starttime;
  return uptime;
}

/* Split a string on a delimiter into a vector of items */
vector<string> LinuxParser::Split(const string &s, const char delimeter) {
    vector<string> items;
    std::stringstream stream;
    stream.str(s);
    string item;
    while (std::getline(stream, item, delimeter)) {
        items.push_back(item);
    }
    return items;
}
