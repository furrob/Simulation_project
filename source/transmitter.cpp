#include "transmitter.h"

void Transmitter::AddPacket(Packet* packet)
{
  buffer_.push(packet);
}
