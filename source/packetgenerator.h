#ifndef PACKETGENERATOR_H
#define PACKETGENERATOR_H

#include "logger.h"
#include "packet.h"
#include "process.h"
#include "transmitter.h"
#include "simulator.h"

#include <cstdlib>

class Transmitter;

//Class used to independently generate new packets and place it in buffers
class PacketGenerator :
  public Process
{
public:

  PacketGenerator(Simulator* simulator, Transmitter* transmitter);

  ~PacketGenerator();//TODO <this

  void Execute() override;

  void Activate(double time) override;

private:
  int id_ = -1;

  Simulator* simulator_ = nullptr;

  //Pointer to transmitter, to which buffer generated packets will go
  Transmitter* transmitter_ = nullptr;

  Logger* logger_ = nullptr;
};
#endif