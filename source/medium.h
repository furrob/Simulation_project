#ifndef MEDIUM_H
#define MEDIUM_H

#include "logger.h"
#include "packet.h"
#include "randomgenerator.h"

#include <functional>
#include <queue>
#include <string>

class Packet;

//Class representing transmission medium
class Medium
{
public:
  typedef std::vector<Packet*> packet_queue;

  Medium(Logger* logger, int seed_TER);

  ~Medium();

  //Returns true if medium is free to transmit.
  bool is_available() { return available_; }

  //Place packet in vector. If there is more than one packet in, raise collision flag
  void Transmit(Packet* packet);

  //Marks channel as not available for transmission
  void Reserve();

  //Dequeue packet with the lowest transmission time, if collision flag is risen, mark packet as collided
  Packet* EndTransmission(int id);

  //Marks medium as free to transmit if no packets in transmission
  void Release();

  bool get_TER();

private:
  //Vector of packets currently being transmitted. If more than one packet is present in
  //this vector, collision occurred.
  packet_queue* packets_ = nullptr;

  //Signals if medium is available for transmission (=queue of packets being transmitted is empty)
  bool available_ = true;

  //Signals if collision occured, set to true when there is more than one packet in packets_
  //queue, false when last packet leaves queue (queue empty)
  bool collision_ = false;

  //For generating channel related errors
  RandomGenerator rand_TER_;

  //Pointer to logger object
  Logger* logger_ = nullptr;
};


#endif
