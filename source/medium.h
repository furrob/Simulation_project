#ifndef MEDIUM_H
#define MEDIUM_H

#include "logger.h"
#include "packet.h"

#include <functional>
#include <queue>

class Packet;

//Class representing transmission medium
class Medium
{
public:
  typedef std::priority_queue<Packet*, std::deque<Packet*>, std::function<bool(Packet*, Packet*)>> packet_queue;

  Medium(Logger* logger);

  ~Medium();

  //Returns true if medium is free to transmit.
  bool is_available() { return available_; }

  //Place packet in queue, make medium not available. If there is more than one packet in, raise collision flag
  void Reserve(Packet* packet);

  //Dequeue packet with the lowest transmission time, if collision flag is risen, mark packet as collided
  Packet* EndTransmission();

  //Marks medium as free to transmit if no packets in transmission
  void Release();

private:
  //Queue of packets currently being transmitted. If more than one packet is present in
  //this queue, collision occurred.
  packet_queue* packets_ = nullptr;

  //Signals if medium is available for transmission (=queue of packets being transmitted is empty)
  bool available_ = true;

  //Signals if collision occured, set to true when there is more than one packet in packets_
  //queue, false when last packet leaves queue (queue empty)
  bool collision_ = false;

  //Pointer to logger object
  Logger* logger_ = nullptr;
};


#endif
