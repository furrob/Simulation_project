#ifndef STATS_H
#define STATS_H

//#include <vector>

struct TxStats
{
  //PACKETS
  int no_packets_in_buffer = 0; //in buffer

  int no_packets_served = 0; //number of packet picked from buffer

  int no_packets_received = 0; //correctly received
  int no_packets_lost = 0; //deleted cuz of too much retransmissions
  int no_retransmissions = 0; //all retransmissions

  //TIME
  int time_in_buffer = 0; //total time spent in buffer
  int time_in_system = 0; //total time spent in system (buffer + transmission) - only correctly received
};

class Stats
{
public:
  Stats(int no_tx, double lambda);
  ~Stats();
  
  //INITIAL PHASE
  //std::vector<double> avg_retransmissions_; //TODO delete later

  void Start(int time);
  void End(int time);

  bool EndOfInitialPhase()
  {
    return end_of_initial_phase;
  }

  void IncPacketsInBuffer(int tx_id);
  void IncPacketsServed(int tx_id);
  void IncPacketsReceived(int tx_id);
  void IncPacketsLost(int tx_id);
  void IncRetransmissions(int tx_id);

  void AddTimeInBuffer(int tx_id, int time);
  void AddTimeInSystem(int tx_id, int time);

  long long PacketsGenerated();
  long long PacketsServed();
  long long PacketsReceived();
  long long PacketsLost();

  double AvgPacketErrorRate();
  double MaxPacketErrorRate();
  double AvgRetransmissionsCount();
  double PacketThroughput();
  double AvgPacketDelay(); //in [ms]
  double AvgPacketWaitingTime(); //in [ms]

private:
  bool end_of_initial_phase = false;

  int no_tx_ = 0; //number of tx in network

  int total_time_span_ = 0;

  TxStats* tx_stats_; //pointer to txstats struct array

};


#endif