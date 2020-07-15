#include "packet.h"


Packet::Packet(Simulator* simulator, Transmitter* transmitter):
  simulator_(simulator), transmitter_(transmitter)
{
  id_ = simulator_->get_network()->get_next_packet_id();

  channel_ = simulator_->get_network()->get_channel();
  logger_ = simulator_->get_logger();

  time_of_generation_ = simulator_->get_clock();

  stats_ = simulator_->get_stats();

  tx_id_ = transmitter_->get_id();

  retransmission_count_ = 0;

#ifdef _DEBUG
  logger_->Debug("PACKET::PACKET ID:" + std::to_string(id_) + " created\n");
#endif
}

Packet::~Packet()
{
  //logger_->Debug("Deleting Packet " + std::to_string(id_) + "\n"); //log
}

void Packet::Execute()
{
  bool active; //flag used to determine if process should go further

#ifdef _DEBUG
  logger_->Debug("PACKET::EXECUTE ID:" + std::to_string(id_) + "\n");
  logger_->IndentForward();
#endif

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
#ifdef _DEBUG
          logger_->Debug("Transmitter ready -> leaving buffer\n");
#endif
          transmitter_->PopFromBuffer();
          transmitter_->set_packet(this);

          transmission_time_ = transmitter_->get_transmission_time();

          state_ = State::MEDIUM_ACCESSING; //If packet can be transmit by transmitter, change state
          active = true; //instead of calling Activate(0);

          stats_->AddTimeInBuffer(tx_id_, simulator_->get_clock() / 10 - time_of_generation_ / 10); //STATS
          stats_->IncPacketsServed(tx_id_);
        }
#ifdef _DEBUG
        else
        {
          logger_->Debug("Transmitter busy - waiting for signal to continue\n");
        }
#endif
        break;
      }

      case State::MEDIUM_ACCESSING:
      {
        if(channel_->is_available())
        {
#ifdef _DEBUG
          logger_->Debug("Channel availability checking -> channel available\n");
#endif
          state_ = State::SENDING;  //Medium available - begin transmission
          active = true;
        }
        else
        {
#ifdef _DEBUG
          logger_->Debug("Channel availability checking -> channel not available\n");
#endif
          Activate(5);  //wait 0.5ms
        }
        break;
      }

      case State::SENDING:
      {
        if(retransmission_count_ <= simulator_->get_network()->get_max_retransmission_count())
        {
#ifdef _DEBUG
          logger_->Debug("Preparing to send\n");
#endif
          channel_->Transmit(this);//"Put" packet into channel and handle collisions
          has_errors_ = channel_->get_TER();//Random chance that packet data is somehow corrupted - default for now is 20%
          state_ = State::IN_TRANSIT;

          last_ = true; //to reserve channel after putting packet into it ( to let collisions occur)
          Activate(0);  //wait to the end of current time
        }
        else
        {
#ifdef _DEBUG
          logger_->Debug("Too many retransmissions -> scheduling packet deletion\n");
#endif
          is_terminated_ = true; //too many retransmissions - deleting from system based on this flag
          stats_->IncPacketsLost(tx_id_);

#ifdef _DEBUG
          logger_->Debug("Signal transmitter to serve next packet from buffer\n");
#endif
          transmitter_->set_packet(nullptr);
          transmitter_->SendNext();
        }
        break;
      }

      case State::IN_TRANSIT:
      {
        last_ = false;
        channel_->Reserve();
#ifdef _DEBUG
        logger_->Debug("Channel reservation, sending\n");
#endif
        state_ = State::VERIFYING;
        Activate(transmission_time_);
        break;
      }

      case State::VERIFYING:
      {
#ifdef _DEBUG
        logger_->Debug("Arriving at receiver\n");
#endif
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

#ifdef _DEBUG
        logger_->Debug("Waiting for ACK\n");
#endif

        last_ = true; //for channel releasing purposes
        Activate(WirelessNetwork::ack_time);
        break;
      }

      case State::RECEIVED:
      {
        last_ = false;
        if(Verify())
        {
#ifdef _DEBUG
          logger_->Debug("ACK received -> scheduling packet deletion\n");
#endif
          channel_->Release(); //free channel after ACK received

          is_terminated_ = true; //If no errors or collision
          stats_->IncPacketsReceived(tx_id_);
          stats_->AddTimeInSystem(tx_id_, simulator_->get_clock()/10 - time_of_generation_/10);

          //INITIAL PHASE stats here
          //stats_->avg_retransmissions_.push_back(stats_->AvgRetransmissionsCount());

#ifdef _DEBUG
          logger_->Debug("Signal transmitter to serve next packet from buffer\n");
#endif
          transmitter_->set_packet(nullptr);
          transmitter_->SendNext();
        }
        else
        {
#ifdef _DEBUG
          logger_->Debug("ACK timeout -> retransmission attempt\n");
#endif
          ++retransmission_count_;

          if(retransmission_count_ <= simulator_->get_network()->get_max_retransmission_count())
            stats_->IncRetransmissions(tx_id_); //test

          state_ = State::MEDIUM_ACCESSING; //Try to retransmit packet
          Activate(transmitter_->get_retransmission_time()); //random time - CRP
        }

        break;
      }

      default:
        logger_->Error("PACKET::EXECUTE Invalid process state\n");
        break;
    }
  } while(active);
#ifdef _DEBUG
  logger_->IndentBack();
#endif
}


void Packet::Activate(int time)
{
  int actual_clock = simulator_->get_clock();
  time_ = actual_clock + time;

#ifdef _DEBUG
  logger_->Debug("Packet " + std::to_string(id_) + " - active in " + 
    std::to_string(static_cast<double>(time) / 10) + "[ms], " +
    std::to_string(static_cast<double>(time_) / 10) + "[ms] global time\n");
#endif

  simulator_->Schedule(this);
}


