#include "simulator.h"

int Simulator::NextSeed(std::vector<int>* seeds)
{
  if(!seeds->empty())
  {
    int temp = seeds->back();
    seeds->pop_back();
    return temp;
  }
  else
  {
    std::cerr << "NO MORE SEEDS\n";
    return -1;
  }
}

Simulator::Simulator(Logger* logger, Stats* stats, double lambda): logger_(logger), lambda_(lambda)
{
  //first small <-> last big
  auto comp = [](Process* l, Process* r)
  {
    return ((l->get_time() == r->get_time()) ? l->is_last() > r->is_last() : l->get_time() > r->get_time()); //that -should- work
  };
  agenda_ = new Agenda(comp);

  stats_ = stats;
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

int Simulator::Init(std::vector<int>* seeds, int max_retransmissions) //todo add seeds
{
#ifdef _DEBUG
  logger_->Debug("SIMULATOR::INITIALIZE\n");
  logger_->IndentForward();
#endif

  //before ANY Acivate() call this must be initialized to 0
  clock_ = 0;

  network_ = new WirelessNetwork(this, logger_, seeds, max_retransmissions);

  network_->Init(33); //<------------------------------------

#ifdef _DEBUG
  logger_->IndentBack();
  logger_->Debug("SIMULATOR::INITIALIZE - DONE\n");
#endif

  return 0;
}

int Simulator::Run(double max_clock, double initial_phase_duration)
{
  long max_clock_i = static_cast<int>(floor(max_clock * 10));
  long initial_phase_duration_i = static_cast<int>(floor(initial_phase_duration * 10));

  is_going_ = true;

  char mode ='a';
  long long iteration = 0;

#ifdef _DEBUG
  logger_->Debug("\nSIMULATION CLOCK LIMIT: " + std::to_string(static_cast<double>(max_clock_i) / 10) + "[ms]\n");
#endif

  //MAIN LOOP
  while(clock_ < max_clock_i)
  {
    //wait for initial phase to end
    if(clock_ >= initial_phase_duration_i)
      stats_->Start(clock_);

    Process* process = agenda_->top();
    agenda_->pop();
    clock_ = process->get_time();
    ++iteration;

    if(mode != 'y') //as long as key pressed is different from 'y', keep asking to press different key to continue to next "step"
    {
      logger_->Info("SIMULATION CLOCK: " + std::to_string(static_cast<double>(clock_) / 10) + "[ms], ITERATION #" +
        std::to_string(iteration) + "\n");//Debug or Info
      logger_->Info("Press [y] to switch to continuous mode, or press any other alphanumeric key to perform next loop iteration ");
      mode = _getch();
      logger_->Info("\n\n");
    }
#ifdef _DEBUG
    logger_->IndentForward();
#endif
    process->Execute();

    //only for packets, generators are -e t e r n a l-
    if(process->is_terminated())
    {
      delete process;
#ifdef _DEBUG
      logger_->Debug("\n");
      logger_->IndentBack();
#endif
      continue;
    }

#ifdef _DEBUG
    logger_->Debug("\n");
    logger_->IndentBack();
#endif
  }
  is_going_ = false;

  stats_->End(clock_);

  return 0;
}

void Simulator::Schedule(Process* process)
{
  agenda_->push(process);
}
