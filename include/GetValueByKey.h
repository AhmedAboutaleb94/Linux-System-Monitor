#ifndef HELPER_PARSER_H
#define HELPER_PARSER_H

#include <fstream>
#include <regex>
#include <string>

#include "linux_parser.h"

using std::string;
using std::vector;

namespace GetValue {
  template <typename S>
  // A template function that can work with any datatype used to get the value from the linux /proc directory by matching it to a key.
  S GetValueByKeySystem(const string &filter, const string &filename){
    string line, key;
    S value;
    std::ifstream stream(LinuxParser::kProcDirectory + filename);
    if(stream.is_open()){
      while(std::getline (stream, line)){
        std::istringstream linestream(line);
        while(linestream >> key >> value){
          if(key == filter){ return value;}
        }
      }
    }
    return value;
  }

  template <typename P>
  // A template member function used to get the value based on a key variable.
  P GetValueByKeyProcess(const string &filter, const string &filename, int &pid){
    string line, key;
    P value;
    std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + filename);
    if(stream.is_open()){
      while(std::getline (stream, line)){
        std::stringstream linestream(line);
        while(linestream >> key >> value){
          if(key == filter){ return value;}
        }
      }
    }
    return value;
  }
}; // namespace GetValue

#endif