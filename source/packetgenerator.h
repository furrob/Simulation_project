#ifndef PACKETGENERATOR_H
#define PACKETGENERATOR_H

#include "process.h"
#include "logger.h"
#include "packet.h"
#include  <string>

class Transmitter;

//Class used to independently generate new packets and place it in buffers
class PacketGenerator :
  public Process
{
public:
  void Execute() override;

  void Activate(double time) override;

private:
  //Time of next packet arrival in network (CGP)
  double generation_time_ = -1;

  //Pointer to transmitter, to which buffer generated packets will go
  Transmitter* transmitter_ = nullptr;

  Logger* logger_ = nullptr;
};
#endif