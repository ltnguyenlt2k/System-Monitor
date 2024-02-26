#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, std::string user, std::string command,
        float cpuUtilization, std::string ram, long int upTime)
    :m_pid(pid), m_user(user), m_command(command), m_cpuUtilization(cpuUtilization), m_ram(ram), m_upTime(upTime){};

// TODO: Return this process's ID
int Process::Pid() const {return m_pid;}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return m_cpuUtilization;}

// TODO: Return the command that generated this process
string Process::Command() const { return m_command; }

// TODO: Return this process's memory utilization
string Process::Ram() const { return m_ram; }

// TODO: Return the user (name) that generated this process
string Process::User() const { return m_user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return m_upTime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const{
  return this->m_cpuUtilization < a.CpuUtilization();
}