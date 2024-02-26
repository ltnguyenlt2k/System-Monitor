#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  float aggregateCPU = 0.0;
  long activeJiffies = LinuxParser::ActiveJiffies();
  long idleJiffies = LinuxParser::IdleJiffies();
  aggregateCPU = (activeJiffies - m_prevActiveJifffies) / float((idleJiffies - m_prevIdleJiffies) + (idleJiffies - m_prevIdleJiffies));
  return aggregateCPU;
}