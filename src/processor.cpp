#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    _idleTime = LinuxParser::IdleJiffies();
    _activeTime = LinuxParser::ActiveJiffies();

    float aTime = (_activeTime - _prevActiveTime) * 1.0;
    float iTime = (_idleTime - _prevIdleTime) * 1.0;

    float utilization = aTime / (aTime + iTime);

    _prevIdleTime = _idleTime;
    _prevActiveTime = _activeTime;

    return utilization;
 }