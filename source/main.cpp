#include "inc.h"

int main(int argc, char *argv[] )
{
  std::cout << "M4 A3\n";

  Logger logger("./../LOG.txt");
  logger.set_flag(Logger::Target::CONSOLE);
  //logger.set_flag(Logger::Target::FILE); <- not quite ready for now (it works, but not humanitarian enough)

  Simulator simulator = Simulator(&logger);
  simulator.Initialize();

  logger.set_flag(Logger::Mode::ERROR);
  logger.set_flag(Logger::Mode::INFO);
  logger.set_flag(Logger::Mode::DEBUG);

  //to alter "generation density" head to generator's Execute() function
  simulator.Run(500); //100ms limit results in ~1000 main loop iterations; 500ms - around 10k

  return 0;
}
  //************************************************************************************
  //auto comp = [](int l, int r) {return l > r; };

  //std::priority_queue<int, std::deque<int>, std::function<bool(int, int)>> que(comp);

  //int tab[] = { 6,3,5,6,8,3,2,6,8,4,2 };

  //for(int i = 0; i < sizeof(tab) / sizeof(int); ++i)
  //{
  //  que.push(tab[i]);
  //}

  //for(int i = 0; i < sizeof(tab) / sizeof(int); ++i)
  //{
  //  std::cout << que.top() << " ";
  //  //que.pop();
  //}

  //if(que.empty())
  //  std::cout << "\nkoniec\n";