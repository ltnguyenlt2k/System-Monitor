#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();
  vector<int> m_pids = LinuxParser::Pids();
  for(int processPid : m_pids)
  {
    string processUserName = LinuxParser::User(processPid);
    string processCommand = LinuxParser::Command(processPid);
    string processRam = LinuxParser::Ram(processPid);
    long int processUpTime = LinuxParser::UpTime() - LinuxParser::UpTime(processPid);
    float processCpuUtilization = LinuxParser::ActiveJiffies(processPid) / float(LinuxParser::ActiveJiffies());
    Process m_process(processPid, processUserName, processCommand, processCpuUtilization, processRam, processUpTime);
    processes_.push_back(m_process);
  }
  std::sort(processes_.begin(), processes_.end(), [](const Process& firstProcess, const Process& secondProcess){
      return secondProcess < firstProcess;
  });
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }