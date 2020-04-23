#include "packet.h"

#include "wirelessnetwork.h"

double Packet::GetCRPTime()
{
  int r_limit = 1 << retransmission_count_;
  int r = rand() % r_limit;
  return transmission_time_ * r;
}

void Packet::Execute()
{
  switch(state_)
  {
    //Initial state, packet already placed in transmitter's input buffer, waiting to be served
    case State::IN_BUFFER:
    {
      if(!(transmitter_->is_busy()))
      {
        logger_->Debug("Leaving buffer\n");
        transmitter_->set_packet(this);
        state_ = State::MEDIUM_ACCESSING; //If packet can be transmit by transmitter, change state
        Activate(0);  //Indirectly call Execute() again
      }
      break;
    }
    
    case State::MEDIUM_ACCESSING:
    {
      if(channel_->is_available())
      {
        logger_->Debug("Channel available\n");
        state_ = State::IN_TRANSIT;  //Medium available - begin transmission
        Activate(0);
      }
      else
        Activate(0.5);  //wait 0.5ms

      break;
    }
      
    case State::IN_TRANSIT:
    {
      if(retransmission_count_ <= WirelessNetwork::max_retransmission_count_)
      {
        logger_->Debug("Sending\n");
        channel_->Reserve(this);//"Put" packet into channel and handle collisions
        this->TransmissionError(70);//Random chance that packet data is somehow corrupted
        state_ = State::VERIFYING;
        Activate(transmission_time_);  //wait for transmission time
      }
      else
      {
        is_terminated_ = true; //too many retransmissions - deleting from system based on this flag - maybe Activate(0) and then before
        //calling Execute() check this flag and delete if set - then signal to transmitter that it can process next packet from buffer
      }
      break;
    }
      
    case State::VERIFYING:
    {
      logger_->Debug("Arrival at receiver\n");
      ; //Take out packet from channel
      if(channel_->EndTransmission() != this) //make sure its right one
      {
        logger_->Error("PACKET::EXECUTE Wrong packet received\n");
        break;
      }
      state_ = State::RECEIVED;

      if(!this->Verify()) //if has errors, release channel now, if no, wait for ack to reach transmitter
      {
        channel_->Release();
      }

      Activate(Receiver::ack_time);
      break;
    }
      
    case State::RECEIVED:
    {
      if(Verify())
      {
        logger_->Debug("Successful transmission\n");

        channel_->Release(); //free channel after ACK received

        is_terminated_ = true; //If no errors or collision
        //signal to first packet waiting in buffer will be in packet destructor
        Activate(0);
      }
      else
      {
        logger_->Debug("ACK timeout -> retransmission attempt\n");
        ++retransmission_count_;
        state_ = State::MEDIUM_ACCESSING; //Try to retransmit packet
        Activate(this->GetCRPTime()); //random time - CRP
      }

      break;
    }
      
    default:
      logger_->Error("PACKET::EXECUTE Invalid process state\n");
      break;
  }
}

void Packet::Activate(double time)
{
  //if(time != 0)
  //  logger_->Debug("Wait\n");
  //logger_->Debug("Place pointer to packet on Future Event List\n");
  logger_->Debug("- - - - - - - - -\n");
}

//TEST
Packet::Packet(Transmitter* transmitter, Receiver* receiver, Medium* channel, Logger* logger):
  transmitter_(transmitter), receiver_(receiver), channel_(channel), logger_(logger)
{

}
