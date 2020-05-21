#include "wirelessnetwork.h"
//todo add various seeds as parameters, maybe in form of vector?
WirelessNetwork::WirelessNetwork(Simulator* simulator, Logger* logger, int seed): logger_(logger)
{
  channel_ = new Medium(logger_, seed); //todo different seed!

  generators_.clear();

  logger_->Debug("WIRELESSNETWORK::WIRELESSNETWORK\n");
  logger_->IndentForward();

  //Fill vector with generators (simulation object pointer needed)
  for(int i = 0; i < terminal_pairs_count; ++i)
  {
    auto transmitter = new Transmitter(i, seed + i, seed + 10 * i);
    generators_.push_back(new PacketGenerator(simulator, transmitter, seed * 5 + i)); //todo different seeds!
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
  logger_->Debug("WIRELESSNETWORK::INITIALIZE\n");
  logger_->IndentForward();

  //Place all generators on agenda
  for(auto it = generators_.begin(); it != generators_.end(); ++it)
  {
    (*it)->Init();
  }

  logger_->IndentBack();
  logger_->Debug("WIRELESSNETWORK::INITIALIZE - DONE\n");

  return 0;
}

