#ifndef PACKETGENERATOR_H
#define PACKETGENERATOR_H

#include "logger.h"
#include "packet.h"
#include "process.h"
#include "transmitter.h"
#include "simulator.h"
#include "randomgenerator.h"
//#include "stats.h"

#include <cstdlib>

class Transmitter;
class Simulator;
class Process;

//Class used to independently generate new packets and place it in buffers
class PacketGenerator :
  public Process
{
public:

  PacketGenerator(Simulator* simulator, Transmitter* transmitter, int seed_CGP);

  ~PacketGenerator();

  void Execute() override;

  //Reschedule with updated time - assumption -> if Activate() is called,
  //process *is* already removed from Agenda
  void Activate(int time) override;

  //Places process on agenda
  void Init();

private:
  Simulator* simulator_ = nullptr;

  RandomGenerator rand_CGP_;

  //Pointer to transmitter, to which buffer generated packets will go
  Transmitter* transmitter_ = nullptr;

  Logger* logger_ = nullptr;
};
#endif