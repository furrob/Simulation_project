#ifndef PACKET_H
#define PACKET_H

class Transmitter;
class Receiver;
struct Medium;

//Class representing a single packet in network
class Packet
{
private:
  //Transmitter - orign of this packet
  Transmitter* transmitter_;

  //Receiver - destination of this packet
  Receiver* receiver_;

  //Time needed for packet to travel between tx-rx
  double transmission_time_;

  //Flag to indicate whenever packet has errors
  bool has_errors_;

  //Flag to indicate collision
  bool collided_;

  //Flag used to chceck if packet was transmitted correctly
  bool ack_;

  //Counter to keep track of # of retransmissions
  int retransmission_count;

  //Pointer to transmission medium used to transmit packet
  Medium* channel_;
};
#endif