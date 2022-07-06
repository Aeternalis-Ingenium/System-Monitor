#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "linux_parser.h"

// An example of how to read data from the filesystem
std::string LinuxParser::OperatingSystem() {
  std::string line;
  std::string key;
  std::string value;
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

// An example of how to read data from the filesystem
std::string LinuxParser::Kernel() {
  std::string os, kernel, version;
  std::string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
std::vector<int> LinuxParser::Pids() {
  std::vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      std::string filename(file->d_name);

      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = std::stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
	std::string line, key, unit;
	float data;
  	std::vector<float> memory_data;
	std::ifstream memstream(kProcDirectory + kMeminfoFilename);

  	if (memstream.is_open()) {
		while (std::getline(memstream, line)) {
        	std::istringstream linestream(line);
          	linestream >> key >> data >> unit;
          	memory_data.push_back(data);
        }
    }

	return (memory_data.at(MemoryTypes::kTotalMem_)	-
           	memory_data.at(MemoryTypes::kFreeMem_)	-
           	memory_data.at(MemoryTypes::kBufferMem_)-
           	memory_data.at(MemoryTypes::kCachedMem_))/
      		memory_data.at(MemoryTypes::kTotalMem_);
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  	std::string line;
  	long uptime{0};
  	std::ifstream uptimestream(kProcDirectory + kUptimeFilename);

  	if (uptimestream.is_open()) {
    	std::getline(uptimestream, line);
      	std::istringstream linestream(line);
      	linestream >> uptime;
    }

  	return uptime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
	std::string line, cpu_str;
  	long jiffy, jiffies{0};
  	std::ifstream filestream(kProcDirectory + kStatFilename);

  	if (filestream.is_open()) {
    	std::getline(filestream, line);
      	std::istringstream linestream(line);
      	linestream >> cpu_str;

      	while (linestream >> jiffy) {
        	jiffies += jiffy;
        }
    }

  	return jiffies;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
	std::string key, line;
  	long int jiffy{0}, active_jiffies{0};
  	std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);

  	if (filestream.is_open()) {
    	std::getline(filestream, line);
      	std::istringstream linestream(line);

      	for (int i = 0; i < 13; i++) {
        	linestream >> key;
        }

      	for (int i = 0; i < 4; i++) {
        	linestream >> jiffy;
          	active_jiffies += jiffy;
        }
    }

  	return active_jiffies;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
	std::vector<long> active_jiffies = CpuUtilization();

	return	active_jiffies.at(CPUStates::kUser_) + active_jiffies.at(CPUStates::kNice_) +
      		active_jiffies.at(CPUStates::kSystem_) + active_jiffies.at(CPUStates::kIRQ_) +
      		active_jiffies.at(CPUStates::kSoftIRQ_) + active_jiffies.at(CPUStates::kSteal_);
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
	std::vector<long> idle_jiffies = CpuUtilization();

  	return idle_jiffies.at(CPUStates::kIdle_) + idle_jiffies.at(CPUStates::kIOwait_);
}

// Read and return CPU utilization
std::vector<long> LinuxParser::CpuUtilization() {
	std::string line, cpu_str;
  	long jiffy;
  	std::vector<long> cpu_data{};
  	std::ifstream filestream(kProcDirectory + kStatFilename);

  	if (filestream.is_open()) {
    	std::getline(filestream, line);
      	std::istringstream linestream(line);
      	linestream >> cpu_str;

      	while (linestream >> jiffy) {
        	cpu_data.push_back(jiffy);
        }
    }

  	return cpu_data;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  	std::string key, line;
  	float process{0.0};
  	std::ifstream filestream(kProcDirectory + kStatFilename);

  	if (filestream.is_open()) {
      	while (std::getline(filestream, line)) {
        	std::istringstream linestream(line);

          	while (linestream >> key >> process) {
            	if (key == "processes") {

                	return process;
                }
            }
        }
    }

  	return process;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  	std::string key, line;
  	int process;
  	std::ifstream filestream(kProcDirectory + kStatFilename);

  	if (filestream.is_open()) {
      	while (std::getline(filestream, line)) {
        	std::istringstream linestream(line);

          	while (linestream >> key >> process) {
            	if (key == "procs_running") {

                	return process;
                }
            }
        }
    }

  	return process;
}

// Read and return the command associated with a process
std::string LinuxParser::Command(int pid) {
	std::string line{};
  	std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  	if (filestream.is_open()) {
    	std::getline(filestream, line);

      	return line;
    }

  	return line;
}

// Read and return the memory used by a process
float LinuxParser::Ram(int pid) {
	std::string key, line;
  	float memory{0.0}, kb_in_1mb{1000};
	std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  	if (filestream.is_open()) {
    	while (std::getline(filestream, line)) {
        	std::istringstream linestream(line);

          	while (linestream >> key >> memory) {
            	if (key == "VmSize:") {

                	return memory / kb_in_1mb;
                }
            }
        }
    }

  	return memory;
}

// Read and return the user ID associated with a process
std::string LinuxParser::Uid(int pid) {
	std::string key, line, uid{};
	std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  	if (filestream.is_open()) {
    	while (std::getline(filestream, line)) {
        	std::istringstream linestream(line);

          	while (linestream >> key >> uid) {
            	if (key == "Uid:") {

                	return uid;
                }
            }
        }
    }

  	return uid;
}

// Read and return the user associated with a process
std::string LinuxParser::User(int pid) {
	std::string line, name{("x:" + Uid(pid))};
  	std::ifstream passwdstream(kPasswordPath);
	
  	while (std::getline(passwdstream, line)) {
		if (line.find(name) != std::string::npos) {
          int position = line.find(":");
          auto result = line.substr(0, position);

          return result.substr(0, 5);
		}
	}
  	
  	return 0;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
	std::string key, line;
  	long uptime{0};
  	std::vector<std::string> values;
  	std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);

  	if (filestream.is_open()) {
    	std::getline(filestream, line);
      	std::istringstream linestream(line);

		for (int i = 0; i < 21; i++) {
        	linestream >> key;
        }

      	linestream >> uptime;
    }

  	try {
    	uptime = uptime / sysconf(_SC_CLK_TCK);
    } catch (...) {
    	uptime = uptime;
    }

  	return uptime;
}
