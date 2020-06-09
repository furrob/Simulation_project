#include "transmitter.h"

Transmitter::Transmitter(int id, int seed_CTP, int seed_R): id_(id), rand_CTP_(seed_CTP), rand_R_(seed_R)
{
  while(!buffer_.empty())
    buffer_.pop();
}

Transmitter::~Transmitter()
{
 /* while(!buffer_.empty())
  {
    Packet* temp = buffer_.front();
    buffer_.pop();
    delete temp;
  }*/
}

int Transmitter::get_transmission_time()
{
  return rand_CTP_.Rand(10, 100); //"integer"
}

int Transmitter::get_retransmission_time()
{
  int r_max = (1 << packet_->get_retransmission_count()) - 1;
  int r = rand_R_.Rand(0, r_max);
  return packet_->get_transmission_time() * r;
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
