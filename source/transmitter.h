#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "packet.h"
#include "randomgenerator.h"

#include <queue>
#include <cmath>

class Packet;

//Class representing a single transmitter (and its components) in network 
class Transmitter
{
public:
  //Identifier of transmitter, pointer to array of seeds and count of seeds (needs 2
  Transmitter(int id, int seed_CTP, int seed_R);

  ~Transmitter();

  int get_id() const
  {
    return id_;
  }

  //Returns true if there is ongoing packet transmission
  bool is_busy() const { return (packet_ == nullptr) ? false : true; }

  //Returns CTP time - random number {10, 20,...,100} [0.1ms]
  int get_transmission_time();

  //Returns CRP time
  int get_retransmission_time();

  //Sets packet_ pointer to currently served packet
  void set_packet(Packet* packet) { packet_ = packet; }

  //Place incoming packet into buffer
  void AddPacket(Packet* packet);

  //Process next packet from buffer - if buffer is empty, packet after being generated automatically checks if it can be send when Execute()
  //called for the first time
  void SendNext();

  //Remove oldest packet from input buffer
  void PopFromBuffer();

private:
  //Transmitter ID
  const int id_ = 0;

  RandomGenerator rand_CTP_;
  RandomGenerator rand_R_;

  //Buffer for packets to transmit
  std::queue<Packet*> buffer_;

  //Pointer to packet being sent
  Packet* packet_ = nullptr;
};
#endif