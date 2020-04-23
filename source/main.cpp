#include <iostream>
#include "wirelessnetwork.h"

int main(int argc, char *argv[] )
{
  std::cout << "M4 A3\n";
  
  WirelessNetwork network;
  Transmitter transmitter;
  Receiver receiver;
  PacketGenerator generator;
  Medium channel;
  
  Logger logger("./../LOG.txt");
  logger.set_mode_flag(Logger::Mode::ERROR);
  logger.set_mode_flag(Logger::Mode::INFO);
  logger.set_mode_flag(Logger::Mode::DEBUG);
  logger.set_target_flag(Logger::Target::CONSOLE);

  Packet packet(&transmitter, &receiver, &channel, &logger);
  
  //TEST TEST TEST TEST TEST
  while(!packet.terminated())
  {
    packet.Execute();
  }

  return 0;
}
