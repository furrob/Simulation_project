#include <iostream>
#include "wirelessnetwork.h"

int main()
{
  std::cout << "Symulacja cyfrowa\n";

  WirelessNetwork network;
  Transmitter transmitter;
  Receiver receiver;
  Packet packet;
  PacketGenerator generator;
  Medium channel;

  return 0;
}
