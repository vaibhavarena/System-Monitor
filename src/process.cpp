#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int x){_pid = x;}
// TODO: Return this process's ID
int Process::Pid() { return _pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const{ 
    long total_time = LinuxParser::ActiveJiffies(_pid);
    long start_time = LinuxParser::UpTime(_pid);
    long up_time = LinuxParser::UpTime();
    long hertz = sysconf(_SC_CLK_TCK);

    long seconds = up_time - (start_time / hertz);

    float cpu_usage = ((total_time * 1.0) / hertz) / seconds;

    return cpu_usage;
 }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(_pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(_pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(_pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    long process_time = LinuxParser::UpTime(_pid); 
    long sys_time = LinuxParser::UpTime();

    return sys_time - process_time;
    }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const{ 
    
    return (a.CpuUtilization() < this->CpuUtilization()); 
}