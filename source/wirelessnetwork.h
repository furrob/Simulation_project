#ifndef WIRELESSNETWORK_H
#define WIRELESSNETWORK_H

#include "medium.h"
#include "logger.h"
#include "transmitter.h"
#include "simulator.h"
#include "packetgenerator.h"

#include <vector>
#include <cstdlib>

class Medium;
class PacketGenerator;
class Simulator;

//Encapsulates wireless transmission system and all of its elements
class WirelessNetwork
{
public:
  static constexpr int ack_time = 10;

 //Number of tx-rx pairs in network
  static const int terminal_pairs_count = 10; //10

  //probability of transmission error occurring
  static constexpr double P = 0.2; 

  WirelessNetwork(Simulator* simulator, Logger* logger, std::vector<int>* seeds, int max_retransmission);

  ~WirelessNetwork();

  int Init(int channel_seed);

  Medium* get_channel() const
  {
    return channel_;
  }

  Logger* get_logger() const
  {
    return logger_;
  }

  int get_max_retransmission_count() const
  {
    return max_retransmission_count_;
  }

  int get_next_packet_id()
  {
    return ++next_packet_id_;
  }

private:
  int max_retransmission_count_ = 0;

  //id of next packet, starting from 0
  int next_packet_id_ = -1;

  //Vector of generators
  std::vector<PacketGenerator*> generators_;

  //Structure representing transmission medium and its state
  Medium* channel_ = nullptr;

  Logger* logger_ = nullptr;
};

#endif