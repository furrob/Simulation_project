#include "medium.h"

void Medium::Reserve(Packet* packet)
{
  packets_.push(packet);

  available_ = false;

  if(packets_.size() > 1)
    collision_ = true;
    
}

Packet* Medium::EndTransmission()
{
  Packet* temp_packet = packets_.top(); //save pointer to first packet
  packets_.pop(); //remove it from queue

  if(temp_packet == nullptr)
  {
    logger_->Error("MEDIUM::ENDTRANSMISSION Packet null pointer reference\n");
    return nullptr;
  }

  temp_packet->set_collision(collision_);

  if(packets_.empty())
  {
    collision_ = false;
  }

  return temp_packet;
}

void Medium::Release()
{
  if(packets_.empty())
  {
    available_ = true;
  }
}
