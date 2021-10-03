#include "processor.h"
#include "linux_parser.h"

Processor::Processor() {
    prevActiveJiffies_ = LinuxParser::ActiveJiffies();;
    prevTotalJiffies_ =  LinuxParser::Jiffies();;
}

// Return the aggregate CPU utilization since the previous call.
float Processor::Utilization() { 
    float activeJiffies = LinuxParser::ActiveJiffies();
    float totalJiffies = LinuxParser::Jiffies();
    float dActiveJiffies = activeJiffies - prevActiveJiffies_;
    float dTotalJiffies = totalJiffies - prevTotalJiffies_;
    float utilization = dTotalJiffies == 0 ? 0 : dActiveJiffies / dTotalJiffies;
    prevActiveJiffies_ = activeJiffies;
    prevTotalJiffies_ = totalJiffies;
    return utilization;
}