#include "process.h"

void Process::Activate(double time)
{
  double actual_clock = simulator_->get_clock();
  time_ = actual_clock + time;

  simulator_->Schedule(this);
}
