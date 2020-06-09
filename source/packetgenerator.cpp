#include "packetgenerator.h"


PacketGenerator::PacketGenerator(Simulator* simulator, Transmitter* transmitter, int seed_CGP) :
  simulator_(simulator), transmitter_(transmitter), rand_CGP_(seed_CGP, simulator_->get_lambda())
{
  time_ = 0;
  id_ = transmitter_->get_id();

  //debug
  last_ = true;

  logger_ = simulator_->get_logger();

#ifdef _DEBUG
  logger_->Debug("PACKETGENERATOR::PACKETGENERATOR ID:" + std::to_string(id_) + " created\n");
#endif
}

PacketGenerator::~PacketGenerator()
{
  delete transmitter_;
}

void PacketGenerator::Execute()
{
#ifdef _DEBUG
  logger_->Debug("PACKETGENERATOR::EXECUTE ID:" + std::to_string(id_) + "\n");
  logger_->IndentForward();
#endif

  //only place where new Packets should be created
  auto temp = new Packet(simulator_, transmitter_);

  transmitter_->AddPacket(temp);
  temp->Activate(0);

  //STATS
  simulator_->get_stats()->IncPacketsInBuffer(transmitter_->get_id());

  Activate(rand_CGP_.RandExp());

#ifdef _DEBUG
  logger_->IndentBack();
#endif
}

void PacketGenerator::Activate(int time)
{
  int actual_clock = simulator_->get_clock();
  time_ = actual_clock + time;

#ifdef _DEBUG
  logger_->Debug("Generator " + std::to_string(id_) + " - active in " +
    std::to_string(static_cast<double>(time) / 10) + "[ms], " +
    std::to_string(static_cast<double>(time_) / 10) + "[ms] global time\n");
#endif

  simulator_->Schedule(this);
}

void PacketGenerator::Init()
{
  Activate(rand_CGP_.RandExp());
}
