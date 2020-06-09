#include "wirelessnetwork.h"

WirelessNetwork::WirelessNetwork(Simulator* simulator, Logger* logger, std::vector<int>* seeds, int max_retransmission): logger_(logger)
{
  channel_ = new Medium(logger_, Simulator::NextSeed(seeds));

  max_retransmission_count_ = max_retransmission;

  generators_.clear();
#ifdef _DEBUG
  logger_->Debug("WIRELESSNETWORK::WIRELESSNETWORK\n");
  logger_->IndentForward();
#endif
  //Fill vector with generators (simulation object pointer needed)
  for(int i = 0; i < terminal_pairs_count; ++i)
  {
    auto transmitter = new Transmitter(i, Simulator::NextSeed(seeds), Simulator::NextSeed(seeds));
    generators_.push_back(new PacketGenerator(simulator, transmitter, Simulator::NextSeed(seeds)));
  }

  logger_->IndentBack();
  //logger_->Debug("\n");
}

WirelessNetwork::~WirelessNetwork()
{
  delete channel_;

  for(auto it = generators_.begin(); it != generators_.end(); ++it)
    delete *it;
}

int WirelessNetwork::Init(int channel_seed)
{
#ifdef _DEBUG
  logger_->Debug("WIRELESSNETWORK::INITIALIZE\n");
  logger_->IndentForward();
#endif
  //Place all generators on agenda
  for(auto it = generators_.begin(); it != generators_.end(); ++it)
  {
    (*it)->Init();
  }

  logger_->IndentBack();
  logger_->Debug("WIRELESSNETWORK::INITIALIZE - DONE\n");

  return 0;
}

