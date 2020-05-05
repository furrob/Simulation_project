#include "inc.h"

int main(int argc, char *argv[] )
{
  std::cout << "M4 A3\n";
  //
  //WirelessNetwork network;
  //Transmitter transmitter;
  ////Receiver receiver; //useless?
  //PacketGenerator generator;
  //Medium channel;
  //
  //Logger logger("./../LOG.txt");
  //logger.set_mode_flag(Logger::Mode::ERROR);
  //logger.set_mode_flag(Logger::Mode::INFO);
  //logger.set_mode_flag(Logger::Mode::DEBUG);
  //logger.set_target_flag(Logger::Target::CONSOLE);
  //
  //auto packet = new Packet(&transmitter, &channel, &logger);
  //
  //TEST TEST TEST TEST TEST
  //while(!packet->terminated())
  //{
  //  packet->Execute();
  //}

  //delete packet;
  //Simulator simulator();

  auto comp = [](int l, int r) {return l > r; };

  std::priority_queue<int, std::deque<int>, std::function<bool(int, int)>> que(comp);

  int tab[] = { 6,3,5,6,8,3,2,6,8,4,2 };

  for(int i = 0; i < sizeof(tab) / sizeof(int); ++i)
  {
    que.push(tab[i]);
  }

  for(int i = 0; i < sizeof(tab) / sizeof(int); ++i)
  {
    std::cout << que.top() << " ";
    //que.pop();
  }

  if(que.empty())
    std::cout << "\nkoniec\n";

  return 0;
}
