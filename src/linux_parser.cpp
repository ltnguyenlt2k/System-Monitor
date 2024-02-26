#include <dirent.h>
#include <unistd.h>
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  long usedMemory = 0;
  long totalMemory = 0;
  float memoryReturn = 0.0;
  string pathFile = kProcDirectory + kMeminfoFilename;
  std::ifstream fileStream(pathFile);
  if(fileStream.is_open())
  {
    string line, key;
    long value = 0;
    while(std::getline(fileStream, line))
    {
      std::istringstream lineStream(line);
      lineStream >> key;
      lineStream >> value;
      if(key == "MemFree:")
      {
        usedMemory = totalMemory - value;
        break;
      }
       totalMemory = value;
    }
  }
  memoryReturn = usedMemory / float(totalMemory);
  return memoryReturn;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long totalTime = 0;
  string pathFile = kProcDirectory + kUptimeFilename;
  std::ifstream fileStream(pathFile);
  if(fileStream.is_open())
  {
    long suspendTime;
    string line, ignore;
    std::getline(fileStream, line);
    std::istringstream stringStream(line);
    stringStream >> suspendTime;
    totalTime = suspendTime;
  }
  return totalTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long totalCapacity = 0;
  for(string value : CpuUtilization())
  {
    totalCapacity += std::stol(value);
  }
  return totalCapacity;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  float cpuUtilization = 0;
  string pathFile = LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename;
  std::ifstream fileStream(pathFile);
  if(fileStream.is_open())
  {
    long uTime, sTime;
    std::string value;
    std::vector<std::string> cpuUtilizationParams;
    while(std::getline(fileStream, value, ' '))
    {
       cpuUtilizationParams.push_back(value);
    }
    uTime = std::stol(cpuUtilizationParams[14]);
    sTime = std::stol(cpuUtilizationParams[15]);
    cpuUtilization = uTime + sTime;
    return cpuUtilization / sysconf(_SC_CLK_TCK);
  }
  return cpuUtilization;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long cpuUser, cpuNice, cpuSystem, cpuIrq, cpuSoftirq, cpuSteal, cpuGuest, cpuGuestNice;
  std::vector<std::string> cpuUsageParameters = CpuUtilization();
  cpuUser = std::stol(cpuUsageParameters[kUser_]);
  cpuNice = std::stol(cpuUsageParameters[kNice_]);
  cpuSystem = std::stol(cpuUsageParameters[kSystem_]);
  cpuIrq = std::stol(cpuUsageParameters[kIRQ_]);
  cpuSoftirq = std::stol(cpuUsageParameters[kSoftIRQ_]);
  cpuSteal = std::stol(cpuUsageParameters[kSteal_]);
  cpuGuest = std::stol(cpuUsageParameters[kGuest_]);
  cpuGuestNice = std::stol(cpuUsageParameters[kGuestNice_]);
  return (cpuUser + cpuNice + cpuSystem + cpuIrq + cpuSoftirq + cpuGuest + cpuGuestNice + cpuSteal);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long cpuIdle, cpuIowait;
  std::vector<std::string> cpuUsageParameters = CpuUtilization();
  cpuIdle = std::stol(cpuUsageParameters[kIdle_]);
  cpuIowait = std::stol(cpuUsageParameters[kIOwait_]);
  return cpuIdle + cpuIowait;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> utilizationData;
  string pathFile = kProcDirectory + kStatFilename;
  std::ifstream fileStream(pathFile);
  if(fileStream.is_open())
  {
    string line, next;
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    while(lineStream >> next)
    {
      if(next != "cpu")
        utilizationData.push_back(next);
    }
  }
  return utilizationData;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int processes = 0;
  string wordPicker;
  string pathFile = kProcDirectory + kStatFilename;
  std::ifstream fileStream(pathFile);
  if(fileStream.is_open())
  {
    string line;
    while(std::getline(fileStream, line))
    {
      std::istringstream lineStream(line);
      lineStream >> wordPicker;
      if(wordPicker == "processes")
      {
        lineStream >> processes;
        break;
      }
    }
  }
  return processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int runningProcessors = 0;
  string wordPicker;
  string pathFile = kProcDirectory + kStatFilename;
  std::ifstream fileStream(pathFile);
  if(fileStream.is_open())
  {
    string line;
    while(std::getline(fileStream, line))
    {
      std::istringstream lineStream(line);
      lineStream >> wordPicker;
      if(wordPicker == "procs_running")
      {
        lineStream >> runningProcessors;
        break;
      }
    }
  }
  return runningProcessors;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string commandLine;
  string pathFile = kProcDirectory + to_string(pid) + kCmdlineFilename;
  std::ifstream fileStream(pathFile);
  if(fileStream.is_open())
  {
    std::getline(fileStream, commandLine);
  }
  return commandLine;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  long utilizedMemory = 0;
  string strReturn;
  string pathFile = kProcDirectory + to_string(pid) + kStatusFilename;
  std::ifstream fileStream(pathFile);
  if(fileStream.is_open())
  {
    string line, key;
    while(std::getline(fileStream, line))
    {
      std::istringstream stringStream(line);
      stringStream >> key >> utilizedMemory;
      if(key == "VmSize:") break;
    }
  }
  strReturn = to_string(int(utilizedMemory/1000.));
  return strReturn;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string userUid;
  string pathFile = kProcDirectory + to_string(pid) + kStatusFilename;
  std::ifstream fileStream(pathFile);
  if(fileStream.is_open())
  {
    string line;
    while(std::getline(fileStream, line))
    {
      string key, value;
      std::istringstream lineStream(line);
      lineStream >> key >> value;
      if(key == "Uid:")
      {
        userUid = value;
        break;
      }
    }
  }
  return userUid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::string userName, userUid;
  // Find UID
  userUid = Uid(pid);
  // Find username
  std::ifstream fileStream(kPasswordPath);
  if(fileStream.is_open())
  {
    string line, ignore, currUid;
    while(std::getline(fileStream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), '-', '_');
      std::istringstream stringStream(line);
      stringStream >> userName >> ignore >> currUid;
      if(currUid == userUid) break;
    }
  }
  return userName;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long upTime = 0;
  string pathFile = kProcDirectory + to_string(pid) + kStatFilename;
  std::ifstream fileStream(pathFile);
  if(fileStream.is_open())
  {
    string value;
    int counter = 0;
    while(std::getline(fileStream, value, ' '))
    {
      if(counter == 21)
      {
        upTime = std::stol(value);
        break;
      }
      counter++;
    }
  }
  return upTime/ sysconf(_SC_CLK_TCK);
}
