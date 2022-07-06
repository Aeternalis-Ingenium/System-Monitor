#include <array>
#include "processor.h"
#include "linux_parser.h"

// Constructor
Processor::Processor()	{ SetJiffiesParsers(prev_); }

// Return the aggregate CPU utilization
float Processor::Utilization() {
	SetJiffiesParsers(current_);
  	SetDeltaTime(current_, prev_);
	
  	const float util_buffer = 0.0000000001;
  	const float utilization = delta_[LinuxParser::CPUTime::kActiveTime_] / (delta_[LinuxParser::CPUTime::kTotalTime_] + util_buffer);

  	current_.swap(prev_);	// Set the current cpu jifies into previous because of passing time
  
    return utilization;
}

long Processor::TotalJiffies() { return current_[LinuxParser::CPUTime::kTotalTime_]; }

void Processor::SetJiffiesParsers(std::array<long, 3>& jiffies) {
	jiffies[LinuxParser::CPUTime::kIdleTime_] = LinuxParser::IdleJiffies();
	jiffies[LinuxParser::CPUTime::kActiveTime_] = LinuxParser::ActiveJiffies();
  	jiffies[LinuxParser::CPUTime::kTotalTime_] = LinuxParser::Jiffies();
}

void Processor::SetDeltaTime(std::array<long, 3>& current_jiffies, std::array<long, 3>& prev_jiffies) {
	for (int i = 0; i < 3; i++) { delta_[i] = current_jiffies[i] - prev_jiffies[i]; }
}

void Processor::SetNewPrevJiffies() {
	for (std::size_t i = 0; i < prev_.size(); i++) { prev_[i] = current_[i]; }
}