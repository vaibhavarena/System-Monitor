#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:

    long _idleTime;
    long _activeTime;
    
    long _prevIdleTime = 0;
    long _prevActiveTime = 0;
};

#endif