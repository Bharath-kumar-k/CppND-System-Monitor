#include "process.h"
#include "linux_parser.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

// constructor 
Process::Process(int pid){
  this->Pid(pid);
  this->User(pid);
  this->Command(pid);
  this->CpuUtilization(pid);
  this->Ram(pid);
  this->UpTime(pid);
};

// TODO: Return this process's ID
int Process::Pid(){ return pid_; }

string Process::User() { return user_; }

string Process::Command() { return command_;}

float Process::CpuUtilization() { return cpu_utilization_; }

string Process::Ram() { return ram_;}

long int Process::UpTime() { return up_time_; }

bool Process::operator>(Process const &a) const {
  return this->cpu_utilization_ > a.cpu_utilization_;
}

bool Process::operator<(Process const &a) const {
  return this->cpu_utilization_ < a.cpu_utilization_;
}

  // LinuxParser::User(Pid());

// Mutators

void Process::Pid(int pid){pid_ = pid;}

void Process::User(int pid){user_ = LinuxParser::User(pid);}

void Process::Command(int pid){command_ = LinuxParser::Command(pid);}

void Process::CpuUtilization(int pid){
  float active_time_process = float(LinuxParser::ActiveJiffies(pid));
  float active_time_processor  = float(LinuxParser::ActiveJiffies());
  cpu_utilization_ = float(active_time_process/active_time_processor);
}

void Process::Ram(int pid){
  ram_ = LinuxParser::Ram(pid);
}

void Process::UpTime(int pid){
  up_time_ = LinuxParser::UpTime() - LinuxParser::UpTime(pid);
}