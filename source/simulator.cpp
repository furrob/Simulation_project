#include "simulator.h"

Simulator::Simulator(Logger* logger): logger_(logger)
{
  //front small <-> back big
  auto comp = [](Process* l, Process* r) {return l->get_time() > r->get_time(); };
  agenda_ = new Agenda(comp);
}

Simulator::~Simulator()
{
  //clear Agenda and delete all objects left on it
  while(!agenda_->empty())
  {
    auto temp = agenda_->top();
    agenda_->pop();
    delete temp; //TODO <- TAKE CARE OF THIS
  }
}

int Simulator::Initialize()
{
  logger_->Debug("SIMULATOR::INITIALIZE\n");
  logger_->IndentForward();

  //before ANY Acivate() call this must be initialized to 0
  clock_ = 0;

  network_ = new WirelessNetwork(this, logger_);

  network_->Initialize();

  logger_->IndentBack();
  logger_->Debug("SIMULATOR::INITIALIZE - DONE\n");

  return 0;
}

int Simulator::Run(double max_clock)
{
  is_going_ = true;

  char mode ='a';
  int iteration = 0;

  logger_->Debug("\nSIMULATION CLOCK LIMIT: " + std::to_string(max_clock) + "[ms]\n");

  while(clock_ < max_clock)
  {
    ++iteration;
    logger_->Debug("SIMULATION CLOCK: " + std::to_string(clock_) + "[ms], ITERATION #" + 
      std::to_string(iteration) + "\n");//Debug of Info
    logger_->IndentForward();

    if(mode != 'y') //as long as key pressed is different from 'y', keep asking to press different key to continue to next "step"
    {
      logger_->Info("Press [y] to switch to continuous mode, or press any other alphanumeric key to perform next loop iteration ");
      mode = _getch();
      logger_->Info("\n\n");
    }
    Process* process = agenda_->top();
    agenda_->pop();

    clock_ = process->get_time();

    //only for packets, generators are eternal
    if(process->is_terminated())
    {
      delete process;
      logger_->Debug("\n");
      logger_->IndentBack();
      continue;
    }

    process->Execute();

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
