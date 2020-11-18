#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
// Member function used to display the elapsed time of a process
std::string ElapsedTime(long time);

// Member function used to display the Uptime of the System
std::string Uptime(long times);
}; // namespace Format

#endif