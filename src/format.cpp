#include <string>

#include "format.h"
using namespace std;


string Format::ElapsedTime(long int time) { 
    long int hours, minutes, seconds; 
    hours = time / 3600;
    minutes = (time / 60) % 60;
    seconds = time % 60;
    string uptime = to_string(hours) + ":" + to_string(minutes) + ":" + to_string(seconds);
    return uptime;
}

string Format::Uptime(long int times){
    long int hours, minutes, seconds, days; 
    days = times/86400;
    hours = (times%86400)/3600;
    minutes = (times%3600)/60;
    seconds = times%60;
    string uptime = to_string(days) + " Days " + ":" + to_string(hours) + " Hours " + ":" + to_string(minutes) + " Minutes " + ":" + to_string(seconds) + " Seconds ";
    return uptime;
}