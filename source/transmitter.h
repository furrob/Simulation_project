#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include <queue>

class Packet;
class PacketGenerator;

//Class representing a single transmitter (and its components) in network 
class Transmitter
{
private:
  //Transmitter ID
  const int id_ = 0;

  //Buffer for packets to transmit
  std::queue<Packet*> packets;

  //Pointer to packet being sent
  Packet* packet_ = nullptr;

  //Time needed for packet to reach destination receiver
  double transmission_time_ = 0;

  //Pointer to packet generator (independently generated packets will go to transmitter's buffer)
  PacketGenerator* generator_;
};
#endif