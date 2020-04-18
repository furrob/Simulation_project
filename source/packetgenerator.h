#ifndef PACKETGENERATOR_H
#define PACKETGENERATOR_H

class Transmitter;

//Class used to independently generate new packets and place it in buffers
class PacketGenerator
{
private:
  //Time of next packet arrival in network (CGP)
  double generation_time_;

  //Pointer to transmitter, to which buffer generated packets will go
  Transmitter* tx_;
};
#endif