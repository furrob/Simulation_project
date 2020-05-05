#include "transmitter.h"

#include "packet.h"

Transmitter::Transmitter(int id, PacketGenerator* packet_generator): generator_(packet_generator), id_(id)
{
  while(!buffer_.empty())
    buffer_.pop();

  get_transmission_time(true);
}

void Transmitter::AddPacket(Packet* packet)
{
  buffer_.push(packet);
}

void Transmitter::SendNext()
{
  if(!buffer_.empty())
  {
    //invoke next packet from buffer
    buffer_.front()->Activate(0);
  }
}
