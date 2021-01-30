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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
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
  string mem_path = LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename;
  std::ifstream stream(mem_path);
  float mem_free = 0.0, mem_total = 0.0;
  string line, mem_var, mem_size;

  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream process_line(line);
      process_line >> mem_var >> mem_size;

      if(mem_var == "MemTotal:")
        mem_total = stof(mem_size);

      if(mem_var == "MemAvailable:")
        mem_free = stof(mem_size);
    }
  }

  return ((mem_total - mem_free) /mem_total); 
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, parser, up_time, idle_time;
    
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
    if(stream.is_open()){
        std::getline(stream, line);
        std::istringstream parser(line);
        parser >> up_time >> idle_time;
    }
    
    return std::stol(up_time);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
    long total_time = 0;

    std::vector<string> values = CpuUtilization();

    for(int i = kUser_; i < kGuestNice_; i++)
    {
      total_time += std::stol(values[i]);
    }
    
    return total_time;
 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string path = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename;
    std::ifstream streamer(path);
    string line, value;
    std::getline(streamer, line);
    long total = 0;
    std::istringstream parser(line);
    int i = 0;
    while(parser >> value)
    {
      if(i >= 13 && i<=16)
        total += std::stol(value);
      
      ++i;
    }
   return total;
 }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    long total_time = Jiffies() - IdleJiffies();

    return total_time;
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long total_time = 0;

    std::vector<string> values = CpuUtilization();

    for(int i = kIdle_; i < kIOwait_; i++)
    {
      total_time += std::stol(values[i]);
    }
    
    return total_time;
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    vector<string> values;
    string line, value;
    if(filestream.is_open()){
        std::getline(filestream, line);
        std::istringstream parser(line);

        while(parser>>value)
        {
          if (value != "cpu")
          values.push_back(value);
        }
      }
    return values;
 }



// Helper function to return Total processes and running processes
int mem_reader(string process_type){
    string line, memory, metric;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if(stream.is_open())
    {
        while(std::getline(stream , line)){
            std::istringstream parser(line);
            parser >> memory >> metric;
            if(memory == process_type)
                return std::stoi(metric);
        }
    }
    return 0;
}
// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return mem_reader("processes"); }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return mem_reader("procs_running"); }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string path = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kCmdlineFilename;
      std::ifstream stream(path);
      string line;
      if(stream.is_open()){
        std::getline(stream, line);
      }
      return line;

    return "";
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string path = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename;

  std::ifstream stream(path);
  string line, uid, value;
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream parser(line);
      parser >> uid >> value;
      if(uid == "VmSize:")
      return to_string(std::stol(value) / 1024);
    }
  }
  return ""; 
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string path = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename;

  std::ifstream stream(path);
  string line, uid, value;
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream parser(line);
      parser >> uid >> value;
      if(uid == "Uid:")
      return value;
    }
  }
  return ""; 
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::ifstream stream(LinuxParser::kPasswordPath);
  string line, uname, x , value;
  string uid_session = LinuxParser::Uid(pid);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream parser(line);

      parser>> uname >> x >> value;
      if(value == uid_session)
      return uname;
    }
  }
   return "";
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string path = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename;
  string line, value;
  std::ifstream streamer(path);
  if(streamer.is_open()){
    std::getline(streamer, line);
    std::istringstream parser(line);
    int i = 0;
    while(parser >> value){
      if(i==21)
        return (std::stol(value)/sysconf(_SC_CLK_TCK));
      i+=1;
    }
  }
    return 0;
 }