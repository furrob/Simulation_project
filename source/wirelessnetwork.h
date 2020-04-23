#ifndef WIRELESSNETWORK_H
#define WIRELESSNETWORK_H


#include <vector>
#include <queue>
#include <string>
#include <fstream>

#include "logger.h"
#include "process.h"
#include "packet.h"
#include "packetgenerator.h"
#include "transmitter.h"
#include "receiver.h"
#include "medium.h"



//Encapsulates wireless transmission system and all of its elements
class WirelessNetwork
{
public:
  //Number of retransmissions before packet rejection
  const static int max_retransmission_count_ = 10;

  WirelessNetwork();

  ~WirelessNetwork();

private:
  //
  int next_packet_id_ = 0;

  //Number of tx-rx pairs in network
  const int terminal_pairs_count_ = 10;

  //Vector of transmitters
  std::vector<Transmitter*> transmitters_;

  //Vector of receivers
  std::vector<Receiver*> receivers_;

  //Structure representing transmission medium and its state
  Medium channel_ = {};
};
#endif