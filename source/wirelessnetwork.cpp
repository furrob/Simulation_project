#include "wirelessnetwork.h"

WirelessNetwork::WirelessNetwork(Simulator* simulator, Logger* logger): logger_(logger)
{
  channel_ = new Medium(logger_);

  transmitters_.clear();

  for(int i=0; i<terminal_pairs_count_; ++i)
  {
    //auto generator= new PacketGenerator()
    //transmitters_.push_back(new Transmitter())
  }

}

WirelessNetwork::~WirelessNetwork()
{

}

