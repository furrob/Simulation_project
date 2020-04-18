#ifndef RECEIVER_H
#define RECEIVER_H

class Packet;

//Representation of a single receiver in network
class Receiver
{
public:
  //Time needed to sent back ACK (constant and the same for every receiver) [ms]
  static constexpr double ack_time = 10.0; 

private:
  //Identifier of receiver
  int id_;

  //Pointer to packet being sent to this receiver
  Packet* packet_;
};
#endif