#include "packetgenerator.h"


PacketGenerator::PacketGenerator(Simulator* simulator, Transmitter* transmitter) :
  simulator_(simulator), transmitter_(transmitter)
{
  time_ = 0;
  id_ = transmitter_->get_id();
}

void PacketGenerator::Execute()
{
  //only place where new Packets should be created
  auto temp = new Packet(simulator_, transmitter_);

  transmitter_->AddPacket(temp);
  temp->Activate(0);

  Activate(static_cast<double>(rand() % 10) + 1); //TEST - "hard coded" next generation time //generate next packet after random time
}

void PacketGenerator::Activate(double time)
{
  
}
