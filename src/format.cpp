#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long hours = seconds/3600;
    long minutes = (seconds % 3600) / 60;
    long sec = (seconds % 3600) % 60;

    string timer = to_string(hours) + ":" + to_string(minutes) + ":" + to_string(sec);

    return timer;
 }