#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "GetValueByKey.h"

using std::stof;
using std::stoi;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float a = GetValue::GetValueByKeySystem<int>(filterMemTotal, kMeminfoFilename);
  float b = GetValue::GetValueByKeySystem<int>(filterMemFree, kMeminfoFilename);
  return (a - b)/a;
}

// Read and return the system uptime
long int LinuxParser::UpTime() {
  string line;
  long int uptime, idle;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
    return (uptime);
  }
  return (uptime);
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, token;
  vector<string> values;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> token) {
      values.push_back(token);
    }
  }
  long jiffies{0};
  if (values.size() > 21) {
    long user = stol(values[13]);
    long kernel = stol(values[14]);
    long children_user = stol(values[15]);
    long children_kernel = stol(values[16]);
    jiffies = user + kernel + children_user + children_kernel;
  }
  return jiffies;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> time = CpuUtilization();
  return (stol(time[CPUStates::kUser_]) + stol(time[CPUStates::kNice_]) +
          stol(time[CPUStates::kSystem_]) + stol(time[CPUStates::kIRQ_]) +
          stol(time[CPUStates::kSoftIRQ_]) + stol(time[CPUStates::kSteal_]) +
          stol(time[CPUStates::kGuest_]) + stol(time[CPUStates::kGuestNice_]));
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> time = CpuUtilization();
  return (stol(time[CPUStates::kIdle_]) + stol(time[CPUStates::kIOwait_]));
}

// Read and return System CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> sv{};
  string line, key, user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice){
      if(key == "cpu"){
        sv.push_back(user);
        sv.push_back(nice);
        sv.push_back(system);
        sv.push_back(idle);
        sv.push_back(iowait);
        sv.push_back(irq);
        sv.push_back(softirq);
        sv.push_back(steal);
        sv.push_back(guest);
        sv.push_back(guestnice);
        return (sv);
      }
    }
  }
  return (sv); 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int a = GetValue::GetValueByKeySystem<int>(filterProcesses, kStatFilename);
  return a;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int a = GetValue::GetValueByKeySystem<int>(filterRunningProcesses, kStatFilename);
  return a;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line, s;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ' ', '_');
    std::istringstream linestream(line);
    linestream >> s;
    replace(s.begin(), s.end(), '_', ' ');
  }
  return s;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  int a = GetValue::GetValueByKeyProcess<int>(filterProcMem, kStatusFilename, pid);
  return to_string(a / 1024);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string a = GetValue::GetValueByKeyProcess<string>(filterUID, kStatusFilename, pid);
  return a;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string line, key, value, other;
  std::ifstream stream(kPasswordPath);
  while(stream.is_open()){
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> value >> other >> key;
    if(key == uid){
      return value;
    }
  }
  return value;
}

// Read and return the uptime of a process
long LinuxParser::ProcessUpTime(int pid) {
  string line, value;
  vector<string> values;
  long up = 0;
  long uptime = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> value){
        values.push_back(value);
      }
      up = std::stol(values[kStarttime_]) / sysconf(_SC_CLK_TCK);
      uptime = LinuxParser::UpTime() - up;
    }
  }
  return uptime;
}

// Read and return the CpuUtilization of a process
float LinuxParser::CpuUtilization(int pid){
  string line, value;
  vector<string> values;
  int hertz = sysconf(_SC_CLK_TCK);
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> value){
        values.push_back(value);
      }      
    }
  }
  //CPUutilization Calculation
  int total_time = stoi(values[kUtime_]) + stoi(values[kStime_]) + stoi(values[kCUtime_]) + stoi(values[kCStime_]);
  int seconds = LinuxParser::UpTime() - (stoi(values[kStarttime_]) / hertz);
  float usage = (((float)total_time / (float)hertz) / (float)seconds);
  return usage;
}