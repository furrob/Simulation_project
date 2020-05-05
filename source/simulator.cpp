#include "simulator.h"

Simulator::Simulator(Logger* logger): logger_(logger)
{
  //front small <-> back big
  auto comp = [](Process* l, Process* r) {return l->get_time() > r->get_time(); };
  agenda_ = new Agenda(comp);
}

int Simulator::Initialize()
{
  network_ = new WirelessNetwork(this, logger_);



  clock_ = 0;

  return 0;
}
