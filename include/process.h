#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  Process(int pid); // Constructor

  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const &a) const;  // Operator Overloading 
  bool operator>(Process const &a) const;  // Operator Overloading 

  // Mutators 
  void Pid(int);
  void User(int);
  void Command(int);
  void CpuUtilization(int);
  void Ram(int);
  void UpTime(int);


  // TODO: Declare any necessary private members
 private:
  int pid_{0};
  std::string user_{""};
  std::string command_{""};
  float cpu_utilization_{0};
  long up_time_{0};
  std::string ram_{""};
};

#endif