#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  string os, kernel, version;
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
        int pid = std::stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

string LinuxParser::keyParserfile(string key, string filepath) {
  string value{""};
  bool search = true;
  string line, temp;
  std::ifstream stream(filepath);
  if (stream.is_open()){
    while(search == true && stream.peek() != EOF){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> temp;
      if(temp == key){
        linestream >> temp;
        value = temp;
        search = false;
      }
    }
  }
  return value;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, temp, value;
  string total_mem_s, free_mem_s;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    for(int i = 0; i<2; ++i){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> temp >> value >> temp;
      if (i==0)
        total_mem_s = value;
      else if (i==1)
        free_mem_s = value;
    }
  }
  float total_mem = std::stof(total_mem_s);
  float free_mem = std::stof(free_mem_s);
  return (total_mem - free_mem) / total_mem;
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> jiffies = CpuUtilization();
  long t_jiffies = 0;
  for(string jiffie : jiffies) {
    t_jiffies += std::stoi(jiffie);
  }
  return t_jiffies;
}


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long a_jiffies = 0;
  a_jiffies = Jiffies() - IdleJiffies();
  return a_jiffies;
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = CpuUtilization();
  return std::stol(jiffies[3]) + std::stol(jiffies[4]); // 3 -> kIdle_ and 4 -> kIOwait_
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> jiffies;
  string jiffie;
  string line,temp;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); 
    linestream >> temp;
    for(int i = 0; i < 10; ++i) {
      linestream >> jiffie;
      jiffies.push_back(jiffie);
    }
  }
  return jiffies; 
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return std::stoi(LinuxParser::keyParserfile("processes", kProcDirectory + kStatFilename));
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return std::stoi(LinuxParser::keyParserfile("procs_running", kProcDirectory + kStatFilename));
 }

long LinuxParser::ActiveJiffies(int pid) {
  long a_jiffies = 0;
  string utime, stime;
  string line, skip;
  std::ifstream stream(kProcDirectory + std::to_string(pid)+ kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); 
    for(int i = 1; i < 14; ++i) {
      linestream >> skip;
    }
    linestream >> utime >> stime;
  }
  a_jiffies = std::atol(utime.c_str()) + std::atol(stime.c_str());
  return a_jiffies;
}

string LinuxParser::Command(int pid) { 
  string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, command);
  }
  return command;
 }

string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "VmSize:") {
          linestream >> value;
          return to_string(int(std::stof(value)/1024)); // converting kB to MB 
        }
      }
    }
  }
  return "0";
  // string ram_in_kb = LinuxParser::keyParserfile("VmSize:", kProcDirectory + "/" + std::to_string(pid) + kStatusFilename); 
  // return to_string(std::stoi(ram_in_kb) * 0.001); // converting kB to MB  
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") { 
          return value;
        }
      }
    }
  }
  return "0";
  // return LinuxParser::keyParserfile("Uid:", kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string name, x, uid;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> x >> uid) {
        if (uid == LinuxParser::Uid(pid)) {
          return name;
        }
      }
    }
  }
  return "unknown"; 
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long ticks = 0;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); 
    for(int i = 1; i < 22; ++i)
      linestream >> skip;
    linestream >> ticks;
  }
  return ticks/sysconf(_SC_CLK_TCK);
}

