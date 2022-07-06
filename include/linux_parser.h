#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
  // Necessary paths into Linux distributions
  const std::string kProcDirectory{"/proc/"};
  const std::string kCmdlineFilename{"/cmdline"};
  const std::string kCpuinfoFilename{"/cpuinfo"};
  const std::string kStatusFilename{"/status"};
  const std::string kStatFilename{"/stat"};
  const std::string kUptimeFilename{"/uptime"};
  const std::string kMeminfoFilename{"/meminfo"};
  const std::string kVersionFilename{"/version"};
  const std::string kOSPath{"/etc/os-release"};
  const std::string kPasswordPath{"/etc/passwd"};

  // System
  float MemoryUtilization();
  long UpTime();
  std::vector<int> Pids();
  int TotalProcesses();
  int RunningProcesses();
  std::string OperatingSystem();
  std::string Kernel();

  // CPU
  enum CPUStates {
    kUser_ = 0,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
  };
  
  // DIfferent CPU time states
  enum CPUTime {
    kIdleTime_ = 0,
  	kActiveTime_,
    kTotalTime_
  };
  
  // Types of memory in OS
  enum MemoryTypes {
  	kTotalMem_ = 0,
    kFreeMem_,
    kAvailableMem_,
    kCachedMem_,
    kBufferMem_
  };

  std::vector<long> CpuUtilization();
  long Jiffies();
  long ActiveJiffies();
  long ActiveJiffies(int pid);
  long IdleJiffies();

  // Processes
  std::string Command(int pid);
  float Ram(int pid);
  std::string Uid(int pid);
  std::string User(int pid);
  long int UpTime(int pid);
};

#endif