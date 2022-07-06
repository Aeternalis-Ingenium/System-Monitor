#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <array>

class Processor {
	public:
  		Processor();										// Constructor
		float Utilization();
		long TotalJiffies();
  
    // Private member functions
  	void SetJiffiesParsers(std::array<long, 3>& jiffies);
  	void SetDeltaTime(std::array<long, 3>& current_jiffies, std::array<long, 3>& prev_jiffies);
  	void SetNewPrevJiffies();

  	// Private member attributes
	std::array<long, 3> current_;								// current CPU Jiffies
  	std::array<long, 3> prev_;									// previous CPU jiffies
  	std::array<float, 3> delta_;								// Delta values
};

#endif