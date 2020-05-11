#include "transmitter.h"

#include "packet.h"

Transmitter::Transmitter(int id): id_(id)
{
  while(!buffer_.empty())
    buffer_.pop();

  get_transmission_time(true);
}

Transmitter::~Transmitter()
{
  while(!buffer_.empty())
  {
    Packet* temp = buffer_.front();
    buffer_.pop();
    delete temp;
  }
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

void Transmitter::PopFromBuffer()
{
  buffer_.pop();
}
