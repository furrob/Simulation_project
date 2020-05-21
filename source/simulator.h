#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "logger.h"
#include "wirelessnetwork.h"
#include "process.h"

#include <queue>
#include <functional>
#include <conio.h> //getch()
#include <string>

class WirelessNetwork;
class Process;

//Main simulation class that contains all the methods and variables to run and manage simulation
class Simulator
{
public:
  typedef std::priority_queue<Process*, std::deque<Process*>, std::function<bool(Process*, Process*)>> Agenda;

  static constexpr int seed_spacing = 10000; //10k

  Simulator(Logger* logger, double lambda);

  ~Simulator();

  WirelessNetwork* get_network() const
  {
    return network_;
  }

  Logger* get_logger() const
  {
    return logger_;
  }

  //Initializes whole system 
  int Init();

  //Start simulation in continuous mode
  int Run(double max_clock = 500);

  //Put new Process on agenda
  void Schedule(Process* process);

  double get_clock() const
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
  double clock_ = -1;

  //Queue of "time suspended" processes
  Agenda* agenda_;

  //Ptr to object that contains all components of network 
  WirelessNetwork* network_ = nullptr;

  //Pointer to logger object
  Logger* logger_ = nullptr;
};

#endif