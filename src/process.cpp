#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

#include "process.h"
#include "linux_parser.h"

// Process Constructor
Process::Process(int pid, long total_jiffies): pid_(pid) {
  	try {
    	SetCPU(total_jiffies);
    } catch (...) {
    	cpu_ = 0;
    }
}

// Set cpu_
void Process::SetCPU(long total_jiffies) {
  	cpu_ = LinuxParser::ActiveJiffies(pid_) * 1.0 / total_jiffies;
}

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_; }

// Return the command that generated this process
std::string Process::Command() const { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
std::string Process::Ram() const { return std::to_string((int)LinuxParser::Ram(pid_)); }

// Return the user (name) that generated this process
std::string Process::User() const { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime() - LinuxParser::UpTime(pid_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return a.CpuUtilization() < this->CpuUtilization(); }