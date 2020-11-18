#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include <vector>

using namespace std;

class Processor {
 public:
    float Utilization();
 private:
   // Members to hold the previous value of the total cpu usage calculated in the previous epoch
    float PrevTotal{0.0}, PrevIdle{0.0};
   // Memeber function used to convert the string vector containing the values to calculate cpu usage to long vector
    vector<long> convertToLong(vector<string> values);
};

#endif