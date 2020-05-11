#include "packet.h"


Packet::Packet(Simulator* simulator, Transmitter* transmitter):
  simulator_(simulator), transmitter_(transmitter)
{
  id_ = simulator_->get_network()->get_next_packet_id();

  channel_ = simulator_->get_network()->get_channel();
  logger_ = simulator_->get_logger();

  logger_->Debug("PACKET::PACKET ID:" + std::to_string(id_) + " created\n");
}

Packet::~Packet()
{
  logger_->Debug("Deleting Packet " + std::to_string(id_) + "\n"); //log
}

double Packet::GetCRPTime()
{
  int r_limit = 1 << retransmission_count_;
  int r = rand() % r_limit;
  return transmission_time_ * r;
}

void Packet::Execute()
{
  bool active; //flag used to determine if process should go further

  logger_->Debug("PACKET::EXECUTE ID:" + std::to_string(id_) + "\n");
  logger_->IndentForward();

  do
  {
    active = false;

    switch(state_)
    {
      //Initial state, packet already placed in transmitter's input buffer, waiting to be served
      case State::IN_BUFFER:
      {
        if(!(transmitter_->is_busy()))
        {
          logger_->Debug("Transmitter ready -> leaving buffer\n");
          transmitter_->PopFromBuffer();
          transmitter_->set_packet(this);

          transmission_time_ = transmitter_->get_transmission_time(true);

          state_ = State::MEDIUM_ACCESSING; //If packet can be transmit by transmitter, change state
          active = true; //instead of calling Activate(0);
        }
        else
        {
          logger_->Debug("Transmitter busy - waiting for signal to continue\n");
        }
        break;
      }

      case State::MEDIUM_ACCESSING:
      {
        if(channel_->is_available())
        {
          logger_->Debug("Channel availability checking -> channel available\n");
          state_ = State::SENDING;  //Medium available - begin transmission
          active = true;
        }
        else
        {
          logger_->Debug("Channel availability checking -> channel not available\n");
          Activate(0.5);  //wait 0.5ms
        }
        break;
      }

      case State::SENDING:
      {
        if(retransmission_count_ <= WirelessNetwork::max_retransmission_count)
        {
          logger_->Debug("Preparing to send\n");
          channel_->Transmit(this);//"Put" packet into channel and handle collisions
          this->TransmissionError();//Random chance that packet data is somehow corrupted - default for now is 20%
          state_ = State::IN_TRANSIT;

          last_ = true; //to reserve channel after putting packet into it ( to let collisions occur)
          Activate(0);  //wait for transmission time
        }
        else
        {
          logger_->Debug("Too many retransmissions -> scheduling packet deletion\n");
          is_terminated_ = true; //too many retransmissions - deleting from system based on this flag

          logger_->Debug("Signal transmitter to serve next packet from buffer\n");
          transmitter_->set_packet(nullptr);
          transmitter_->SendNext();
        }
        break;
      }

      case State::IN_TRANSIT:
      {
        last_ = false;
        channel_->Reserve();
        logger_->Debug("Channel reservation, sending\n");
        state_ = State::VERIFYING;
        Activate(transmission_time_);
        break;
      }

      case State::VERIFYING:
      {
        logger_->Debug("Arriving at receiver\n");
        ; //Take out packet from channel
        if(channel_->EndTransmission(id_) != this) //make sure its right one
        {
          logger_->Error("PACKET::EXECUTE Wrong packet received\n");
          break;
        }
        state_ = State::RECEIVED;

        if(!Verify()) //if has errors, release channel now, if no, wait for ack to reach transmitter
        {
          channel_->Release();
        }

        logger_->Debug("Waiting for ACK\n");

        last_ = true; //for channel releasing purposes
        Activate(WirelessNetwork::ack_time);
        break;
      }

      case State::RECEIVED:
      {
        last_ = false;
        if(Verify())
        {
          logger_->Debug("ACK received -> scheduling packet deletion\n");

          channel_->Release(); //free channel after ACK received

          is_terminated_ = true; //If no errors or collision
          logger_->Debug("Signal transmitter to serve next packet from buffer\n");
          transmitter_->set_packet(nullptr);
          transmitter_->SendNext();
        }
        else
        {
          logger_->Debug("ACK timeout -> retransmission attempt\n");
          ++retransmission_count_;
          state_ = State::MEDIUM_ACCESSING; //Try to retransmit packet
          Activate(GetCRPTime()); //random time - CRP
        }

        break;
      }

      default:
        logger_->Error("PACKET::EXECUTE Invalid process state\n");
        break;
    }
  } while(active);

  logger_->IndentBack();
}


void Packet::Activate(double time)
{
  double actual_clock = simulator_->get_clock();
  time_ = actual_clock + time;

  logger_->Debug("Packet " + std::to_string(id_) + " - active in " + std::to_string(time) + "[ms], " +
    std::to_string(time_) + "[ms] global time\n");

  simulator_->Schedule(this);
}


