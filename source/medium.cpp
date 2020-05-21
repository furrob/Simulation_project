#include "medium.h"

Medium::Medium(Logger* logger, int seed_TER): logger_(logger), rand_TER_(seed_TER)
{
  packets_ = new packet_queue();
}

Medium::~Medium()
{
  while(!packets_->empty())
    packets_->pop_back();

  delete packets_;
}

void Medium::Transmit(Packet* packet)
{
  packets_->push_back(packet);
}

void Medium::Reserve()
{
  available_ = false;

  if(packets_->size() > 1)
  {
    collision_ = true;
    logger_->Debug("Collision - packet corrupted\n");
  }
}

Packet* Medium::EndTransmission(int id)
{
  Packet* temp_packet = nullptr;

  //ids are unique
  for(auto it = packets_->begin(); it != packets_->end(); ++it)
  {
    if((*it)->get_id() == id)
    {
      temp_packet = (*it);
      packets_->erase(it);
      break;
    }
  }

  if(temp_packet == nullptr)
  {
    logger_->Error("MEDIUM::ENDTRANSMISSION Packet null pointer reference\n");
    return nullptr;
  }

  temp_packet->set_collision(collision_);

  return temp_packet;
}

void Medium::Release()
{
  if(packets_->empty())
  {
    available_ = true;
    collision_ = false;
  }
}

bool Medium::get_TER()
{
  return rand_TER_.RandBin(0.2); //80% chances that packet will be alright
}
