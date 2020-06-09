#include "stats.h"

Stats::Stats(int no_tx, double lambda)
{
  no_tx_ = no_tx;
  tx_stats_ = new TxStats[no_tx_];
}

Stats::~Stats()
{
  delete[] tx_stats_;
}

void Stats::Start(int time)
{
  if(end_of_initial_phase == false)
  {
    total_time_span_ = time / 10;
    end_of_initial_phase = true;
  }
}

void Stats::End(int time)
{
  total_time_span_ = time / 10 - total_time_span_;
}

void Stats::IncPacketsInBuffer(int tx_id)
{
  if(tx_id<no_tx_ && end_of_initial_phase)
  {
    tx_stats_[tx_id].no_packets_in_buffer += 1;
  }
}

void Stats::IncPacketsServed(int tx_id)
{
  if(tx_id < no_tx_ && end_of_initial_phase)
  {
    tx_stats_[tx_id].no_packets_served += 1;
  }
}

void Stats::IncPacketsReceived(int tx_id)
{
  if(tx_id < no_tx_ && end_of_initial_phase)
  {
    tx_stats_[tx_id].no_packets_received += 1;
  }
}

void Stats::IncPacketsLost(int tx_id)
{
  if(tx_id < no_tx_ && end_of_initial_phase)
  {
    tx_stats_[tx_id].no_packets_lost += 1;
  }
}

void Stats::IncRetransmissions(int tx_id)
{
  if(tx_id < no_tx_ && end_of_initial_phase)
  {
    tx_stats_[tx_id].no_retransmissions += 1;
  }
}

void Stats::AddTimeInBuffer(int tx_id, int time)
{
  if(tx_id < no_tx_ && end_of_initial_phase)
  {
    tx_stats_[tx_id].time_in_buffer += time;
  }
}

void Stats::AddTimeInSystem(int tx_id, int time)
{
  if(tx_id < no_tx_ && end_of_initial_phase)
  {
    tx_stats_[tx_id].time_in_system += time;
  }
}

long long Stats::PacketsGenerated()
{
  long long tot_packets = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_packets += tx_stats_[i].no_packets_in_buffer;

  return tot_packets;
}

long long Stats::PacketsServed()
{
  long long tot_packets = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_packets += tx_stats_[i].no_packets_served;

  return tot_packets;
}

long long Stats::PacketsReceived()
{
  long long tot_packets_received = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_packets_received += tx_stats_[i].no_packets_received;

  return tot_packets_received;
}

long long Stats::PacketsLost()
{
  long long tot_packets_lost = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_packets_lost += tx_stats_[i].no_packets_lost;

  return tot_packets_lost;
}

double Stats::AvgPacketErrorRate()
{
  double tot_packet_lost = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_packet_lost += tx_stats_[i].no_packets_lost;

  long tot_packets_received = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_packets_received += tx_stats_[i].no_packets_received;

  return (tot_packets_received > 0) ? tot_packet_lost / tot_packets_received : //if there was packets received, compute per
    (tot_packet_lost > 0) ? 1 : 0; //if not, check if there was packets lost
}

double Stats::MaxPacketErrorRate()
{
  double max_per = -1;
  double temp = 0;
  //int tx_id = -1;
  for(int i=0; i<no_tx_; ++i)
  {
    temp = (tx_stats_[i].no_packets_received > 0) ? 
      static_cast<double>(tx_stats_[i].no_packets_lost) / tx_stats_[i].no_packets_received : //if there was packets received, compute per
      (tx_stats_[i].no_packets_lost > 0) ? 1 : 0; //if not, check if there was packets lost

    if(temp > max_per)
      max_per = temp;
  }

  return max_per;
}

double Stats::AvgRetransmissionsCount()
{
  double tot_retransmissions = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_retransmissions += tx_stats_[i].no_retransmissions;

  long tot_packets_served = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_packets_served += tx_stats_[i].no_packets_served;

  return (tot_packets_served > 0) ? tot_retransmissions / tot_packets_served : -1;
}

double Stats::PacketThroughput()
{
  long tot_packets_send = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_packets_send += tx_stats_[i].no_packets_received;

  return (total_time_span_ > 0) ? (static_cast<double>(tot_packets_send) / total_time_span_) * 1000 : -1;
}

double Stats::AvgPacketDelay()
{
  long tot_packets_received = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_packets_received += tx_stats_[i].no_packets_received;

  long long tot_packets_delay = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_packets_delay += tx_stats_[i].time_in_system;

  return (tot_packets_received > 0) ? static_cast<double>(tot_packets_delay) / tot_packets_received : 0;;
}

double Stats::AvgPacketWaitingTime()
{
  long tot_packets_served = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_packets_served += tx_stats_[i].no_packets_received;

  long long tot_packets_waiting = 0;
  for(int i = 0; i < no_tx_; ++i)
    tot_packets_waiting += tx_stats_[i].time_in_buffer;

  return (tot_packets_served > 0) ? static_cast<double>(tot_packets_waiting) / tot_packets_served : 0;
}
