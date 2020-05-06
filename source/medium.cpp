#include "medium.h"

Medium::Medium(Logger* logger): logger_(logger)
{
  auto comp = [](Packet* l, Packet* r) {return l->get_time() > r->get_time(); };
  packets_ = new packet_queue(comp);
}

Medium::~Medium()
{
  while(!packets_->empty())
    packets_->pop();

  delete packets_;
}

void Medium::Reserve(Packet* packet)
{
  packets_->push(packet);

  available_ = false;

  if(packets_->size() > 1)
  {
    collision_ = true;
    logger_->Debug("!!! Collision - " + std::to_string(packets_->size()) + " packets corrupted !!!\n");
  }
}

Packet* Medium::EndTransmission()
{
  Packet* temp_packet = packets_->top(); //save pointer to first packet
  packets_->pop(); //remove it from queue

  if(temp_packet == nullptr)
  {
    logger_->Error("MEDIUM::ENDTRANSMISSION Packet null pointer reference\n");
    return nullptr;
  }

  temp_packet->set_collision(collision_);

  if(packets_->empty())
  {
    collision_ = false;
  }

  return temp_packet;
}

void Medium::Release()
{
  if(packets_->empty())
  {
    available_ = true;
  }
}
