#include "transmitter.h"

Transmitter::Transmitter(int id, int seed_CTP, int seed_R): id_(id), rand_CTP_(seed_CTP), rand_R_(seed_R)
{
  while(!buffer_.empty())
    buffer_.pop();
}

Transmitter::~Transmitter()
{/* if something is in buffer AND agenda - exception happens
  while(!buffer_.empty())
  {
    Packet* temp = buffer_.front();
    buffer_.pop();
    delete temp;
  }*/
}

double Transmitter::get_transmission_time()
{
  return ceil(rand_CTP_.Rand(1, 10)); //"integer"
}

double Transmitter::get_retransmission_time()
{
  double r_max = static_cast<double>((1 << packet_->get_retransmission_count())) - 1;
  double r = rand_R_.Rand(0, r_max);
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
