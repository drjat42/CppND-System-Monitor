#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();  // TODO: See src/processor.cpp

 private:
  float prevActiveJiffies_;
  float  prevTotalJiffies_;
};

#endif