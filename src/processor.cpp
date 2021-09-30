#include "processor.h"
#include "linux_parser.h"


/*
PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle
CPU_Percentage = (totald - idled)/totald
*/

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    return LinuxParser::ActiveJiffies() / (float)LinuxParser::Jiffies();
}