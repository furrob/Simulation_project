#include "inc.h"

//class test
//{
//public:
//
//  test(int time, bool last)
//  {
//    this->time = time;
//    this->last = last;
//  }
//
//  int time = 0;
//  bool last = false;
//};

int main(int argc, char *argv[] )
{
  std::cout << "M4 A3\n";

  Logger logger("./../LOG.txt");
  logger.set_flag(Logger::Target::CONSOLE);
  //logger.set_flag(Logger::Target::FILE); <- not quite ready for now (it works, but not humanitarian enough)

  logger.set_flag(Logger::Mode::DEBUG);

  Simulator simulator = Simulator(&logger);
  simulator.Initialize();

  logger.set_flag(Logger::Mode::ERROR);
  logger.set_flag(Logger::Mode::INFO);
  logger.set_flag(Logger::Mode::DEBUG);

  //to alter "generation density" head to generator's Execute() function
  simulator.Run(5000); //100ms limit results in ~1000 main loop iterations; 500ms - around 10k

  return 0;

  //*********************************

  //int tabI[] = { 0, 0, 2, 2, 4, 4, 4, 4, 1, 2, 5, 0, 5 };
  //bool tabB[] = { 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0 };

  //auto comp = [](test* l, test* r)
  //{
  //  return ((l->time == r->time) ? l->last > r->last : l->time > r->time); //that -should- work
  //  //return l->time > r->time;
  //};

  //std::priority_queue<test*, std::deque<test*>, std::function<bool(test*, test*)>> que(comp);

  //for(int i = 0; i < sizeof(tabI) / sizeof(int); ++i)
  //{
  //  que.push(new test(tabI[i], tabB[i]));
  //}
  //
  //for(int i = 0; !que.empty(); ++i)
  //{
  //  std::cout << i << " œ " << que.top()->time << " " << que.top()->last << "\n";
  //  que.pop();
  //}

  //return 0;
}
  //************************************************************************************
  //auto comp = [](int l, int r) {return l > r; };

  //std::priority_queue<int, std::deque<int>, std::function<bool(int, int)>> que(comp);

  //int tabI[] = { 0,0,2,2,4,4,4,4,1,2,5,0,5 };
  //bool tabB[] = [
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