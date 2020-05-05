#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "packet.h"
#include "packetgenerator.h"

#include <queue>

class Packet;
class PacketGenerator;

//Class representing a single transmitter (and its components) in network 
class Transmitter
{
public:

  Transmitter(int id, PacketGenerator* packet_generator);

  ~Transmitter(); //TODO write destructor

  int get_id() const
  {
    return id_;
  }

  //Returns true if there is ongoing packet transmission
  bool is_busy() const { return (packet_ == nullptr) ? false : true; }

  //Returns previously drawn transmission time
  //generate_next = true draws next random time
  double get_transmission_time(bool generate_next = false)
  {
    const double temp_time = transmission_time_;
    if(generate_next)
      transmission_time_ = static_cast<double>(rand() % 10) + 1; //random number <1;10> - (CTP)
    return temp_time;;
  }

  //Sets packet_ pointer to currently served packet
  void set_packet(Packet* packet) { packet_ = packet; }

  //Place incoming packet into buffer
  void AddPacket(Packet* packet);

  //Process next packet from buffer - if buffer is empty, packet after being generated automatically checks if it can be send when Execute()
  //called for the first time
  void SendNext();

private:
  //Transmitter ID
  const int id_ = 0;

  //Buffer for packets to transmit
  std::queue<Packet*> buffer_;

  //Pointer to packet being sent
  Packet* packet_ = nullptr;

  //Time needed for packet to reach destination receiver
  double transmission_time_ = 0;

  //Pointer to packet generator (independently generated packets will go to transmitter's buffer)
  PacketGenerator* generator_ = nullptr;
};
#endif