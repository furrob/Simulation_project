#ifndef WIRELESSNETWORK_H
#define WIRELESSNETWORK_H

#include "medium.h"
#include "logger.h"
#include "transmitter.h"
#include "simulator.h"

#include <vector>

class Medium;
class Transmitter;

//Encapsulates wireless transmission system and all of its elements
class WirelessNetwork
{
public:
  static constexpr double ack_time = 10.0;

  //Number of retransmissions before packet rejection
  const static int max_retransmission_count_ = 10;

 //Number of tx-rx pairs in network
  const int terminal_pairs_count_ = 10;

  WirelessNetwork(Simulator* simulator, Logger* logger);

  ~WirelessNetwork();


  Medium* get_channel() const
  {
    return channel_;
  }

  Logger* get_logger() const
  {
    return logger_;
  }

  int get_next_packet_id()
  {
    return ++next_packet_id_;
  }

private:
  //id of next packet, starting from 0
  int next_packet_id_ = -1;

  //Vector of transmitters
  std::vector<Transmitter*> transmitters_;

  //Vector of receivers
  //std::vector<Receiver*> receivers_; //not needed

  //Structure representing transmission medium and its state
  Medium* channel_ = nullptr;

  Logger* logger_ = nullptr;
};

#endif