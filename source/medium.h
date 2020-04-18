#ifndef MEDIUM_H
#define MEDIUM_H

#include <queue>

class Packet;

//Structure representing transmission medium
struct Medium
{
private:
  //Queue of packets currently being transmitted. If more than one packet is present in
  //this queue, collision occurred.
  std::queue<Packet*> packets;

  //Signals if medium is available for transmission (=queue of packets being transmitted is empty)
  bool is_available;
};
#endif
