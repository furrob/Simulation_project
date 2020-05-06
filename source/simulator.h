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

  Simulator(Logger* logger);

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
  int Initialize();

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
private:
  //Indicates if simulation is running
  bool is_going_ = false;

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