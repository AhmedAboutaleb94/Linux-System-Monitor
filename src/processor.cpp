#include "processor.h"
#include "linux_parser.h"
//

// TODO: Return the aggregate CPU utilization
float Processor::Utilization(){
    vector<long> cpuvalues = Processor::convertToLong(LinuxParser::CpuUtilization());
    float usertime = cpuvalues[LinuxParser::kUser_] - cpuvalues[LinuxParser::kGuest_];
    float nicetime = cpuvalues[LinuxParser::kNice_] - cpuvalues[LinuxParser::kGuestNice_];
    float idlealltime = cpuvalues[LinuxParser::kIdle_] + cpuvalues[LinuxParser::kIOwait_];
    float systemalltime = cpuvalues[LinuxParser::kSystem_] + cpuvalues[LinuxParser::kIRQ_] + cpuvalues[LinuxParser::kSoftIRQ_];
    float virtalltime = cpuvalues[LinuxParser::kGuest_] + cpuvalues[LinuxParser::kGuestNice_];
    float totaltime = usertime + nicetime + systemalltime + idlealltime + cpuvalues[LinuxParser::kSteal_] + virtalltime;

    float totald = totaltime - PrevTotal;
    float idled = idlealltime - PrevIdle;
    float usage = (totald - idled) / totald;
    
    PrevTotal = totald;
    PrevIdle = idled; 
    return usage;
}

vector<long> Processor::convertToLong(vector<string> values){
    vector<long> converted_values{};
    for(uint i = 0; i < values.size(); i++){
        converted_values.push_back(stol(values[i]));
    }
    return converted_values;
}