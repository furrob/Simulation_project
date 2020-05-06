#include "wirelessnetwork.h"

WirelessNetwork::WirelessNetwork(Simulator* simulator, Logger* logger): logger_(logger)
{
  channel_ = new Medium(logger_);

  generators_.clear();

  logger_->Debug("WIRELESSNETWORK::WIRELESSNETWORK\n");
  logger_->IndentForward();

  //Fill vector with generators (simulation object pointer needed)
  for(int i = 0; i < terminal_pairs_count; ++i)
  {
    auto transmitter = new Transmitter(i);
    generators_.push_back(new PacketGenerator(simulator, transmitter));
  }

  logger_->IndentBack();
  //logger_->Debug("\n");
}

WirelessNetwork::~WirelessNetwork()
{
  //TODO this^
  delete channel_;

  for(auto it = generators_.begin(); it != generators_.end(); ++it)
    delete *it;
}

int WirelessNetwork::Initialize()
{
  logger_->Debug("WIRELESSNETWORK::INITIALIZE\n");
  logger_->IndentForward();

  //Place all generators on agenda
  for(auto it = generators_.begin(); it != generators_.end(); ++it)
  {
    double time = static_cast<double>(rand() % 5) + 1; //TEST 1-5ms
    (*it)->Activate(time);
  }

  logger_->IndentBack();
  logger_->Debug("WIRELESSNETWORK::INITIALIZE - DONE\n");

  return 0;
}

