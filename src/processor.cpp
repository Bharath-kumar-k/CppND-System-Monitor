#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float total_jiffies = float(LinuxParser::Jiffies());
    float active_jiffies = float(LinuxParser::ActiveJiffies());
    return active_jiffies/total_jiffies;
 }