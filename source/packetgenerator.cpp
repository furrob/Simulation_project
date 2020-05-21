#include "packetgenerator.h"


PacketGenerator::PacketGenerator(Simulator* simulator, Transmitter* transmitter, int seed_CGP) :
  simulator_(simulator), transmitter_(transmitter), rand_CGP_(seed_CGP, simulator_->get_lambda())
{
  time_ = 0;
  id_ = transmitter_->get_id();

  logger_ = simulator_->get_logger();

  logger_->Debug("PACKETGENERATOR::PACKETGENERATOR ID:" + std::to_string(id_) + " created\n");
}

PacketGenerator::~PacketGenerator()
{
  delete transmitter_;
}

void PacketGenerator::Execute()
{
  logger_->Debug("PACKETGENERATOR::EXECUTE ID:" + std::to_string(id_) + "\n");
  logger_->IndentForward();

  //only place where new Packets should be created
  auto temp = new Packet(simulator_, transmitter_);

  transmitter_->AddPacket(temp);
  temp->Activate(0);

  Activate(rand_CGP_.RandExp());

  logger_->IndentBack();
}

void PacketGenerator::Activate(double time)
{
  double actual_clock = simulator_->get_clock();
  time_ = actual_clock + time;

  logger_->Debug("Generator " + std::to_string(id_) + " - active in " + std::to_string(time) + "[ms], " +
    std::to_string(time_) + "[ms] global time\n");

  simulator_->Schedule(this);
}

void PacketGenerator::Init()
{
  Activate(rand_CGP_.RandExp());
}
