#ifndef RECEIVER_H
#define RECEIVER_H

class Packet;

//Representation of a single receiver in network
class Receiver
{
public:
  //Time needed to sent back ACK (constant and the same for every receiver) [ms]
  static constexpr double ack_time = 10.0; 

  //Check packets data integrity flags and sets ACK to true or false accordingly.
  //void CheckPacket(Packet* packet);

  //void set_packet(Packet* packet) { packet_ = packet; }

private:
  //Identifier of receiver
  int id_;

  //Pointer to packet being sent to this receiver
  Packet* packet_;
};
#endif