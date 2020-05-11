#include "medium.h"

Medium::Medium(Logger* logger): logger_(logger)
{
  //auto comp = [](Packet* l, Packet* r) {return l->get_time() > r->get_time(); };
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

  //available_ = false;


}

void Medium::Reserve()
{
  available_ = false;

  if(packets_->size() > 1)
  {
    collision_ = true;
    logger_->Debug("Collision - " + std::to_string(packets_->size()) + " packets corrupted\n"); //<- when reserving
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
