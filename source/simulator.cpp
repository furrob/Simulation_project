#include "simulator.h"

Simulator::Simulator(Logger* logger, double lambda): logger_(logger), lambda_(lambda)
{
  //front small <-> back big
  auto comp = [](Process* l, Process* r)
  {
    return ((l->get_time() == r->get_time()) ? l->is_last() > r->is_last() : l->get_time() > r->get_time()); //that -should- work
  };
  agenda_ = new Agenda(comp);
}

Simulator::~Simulator()
{
  //clear Agenda and delete all objects left on it
  while(!agenda_->empty())
  {
    auto temp = agenda_->top();
    agenda_->pop();
    delete temp;
  }
}

int Simulator::Init() //todo add seeds
{
  logger_->Debug("SIMULATOR::INITIALIZE\n");
  logger_->IndentForward();

  //before ANY Acivate() call this must be initialized to 0
  clock_ = 0;

  network_ = new WirelessNetwork(this, logger_, 997);

  network_->Init(33); //<------------------------------------

  logger_->IndentBack();
  logger_->Debug("SIMULATOR::INITIALIZE - DONE\n");

  return 0;
}

int Simulator::Run(double max_clock)
{
  is_going_ = true;

  char mode ='a';
  int iteration = 0;

  logger_->Info("\nSIMULATION CLOCK LIMIT: " + std::to_string(max_clock) + "[ms]\n");

  while(clock_ < max_clock)
  {

    Process* process = agenda_->top();
    agenda_->pop();
    clock_ = process->get_time();
    ++iteration;

    logger_->Info("SIMULATION CLOCK: " + std::to_string(clock_) + "[ms], ITERATION #" + 
      std::to_string(iteration) + "\n");//Debug of Info
    logger_->IndentForward();

    if(mode != 'y') //as long as key pressed is different from 'y', keep asking to press different key to continue to next "step"
    {
      logger_->Info("Press [y] to switch to continuous mode, or press any other alphanumeric key to perform next loop iteration ");
      mode = _getch();
      logger_->Info("\n\n");
    }

    process->Execute();

    //only for packets, generators are -e t e r n a l-
    if(process->is_terminated())
    {
      delete process;
      logger_->Debug("\n");
      logger_->IndentBack();
      continue;
    }

    logger_->Debug("\n");
    logger_->IndentBack();
  }
  is_going_ = false;
  return 0;
}

void Simulator::Schedule(Process* process)
{
  agenda_->push(process);
}
