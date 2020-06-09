#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "logger.h"
#include "wirelessnetwork.h"
#include "process.h"
#include "stats.h"

#include <queue>
#include <functional>
#include <conio.h> //getch()
#include <string>
#include <vector>

class WirelessNetwork;
class Process;

//Main simulation class that contains all the methods and variables to run and manage simulation
class Simulator
{
public:
  typedef std::priority_queue<Process*, std::deque<Process*>, std::function<bool(Process*, Process*)>> Agenda;

  //static constexpr int seed_spacing = 10000; //10k

  static int NextSeed(std::vector<int>* seeds);

  Simulator(Logger* logger, Stats* stats, double lambda);

  ~Simulator();

  WirelessNetwork* get_network() const
  {
    return network_;
  }

  Logger* get_logger() const
  {
    return logger_;
  }

  Stats* get_stats() const
  {
    return stats_;
  }

  //Initializes whole system 
  int Init(std::vector<int>* seeds, int max_retransmission);

  //Start simulation in continuous mode
  int Run(double max_clock, double initial_phase_duration);

  //Put new Process on agenda
  void Schedule(Process* process);

  long get_clock() const
  {
    return clock_;
  }

  bool is_going() const
  {
    return is_going_;
  }

  double get_lambda() const
  {
    return lambda_;
  }

private:
  //Indicates if simulation is running
  bool is_going_ = false;

  //can be set through input parameter to *.exe file
  double lambda_; 

  //Actual simulation time [ms]
  long clock_ = -1;

  //Queue of "time suspended" processes
  Agenda* agenda_;

  //Ptr to object that contains all components of network 
  WirelessNetwork* network_ = nullptr;

  //class used to manage statistics
  Stats* stats_ = nullptr;

  //Pointer to logger object
  Logger* logger_ = nullptr;
};

#endif