#ifndef PACKET_H
#define PACKET_H

#include "process.h"
#include "transmitter.h"
#include "medium.h"
#include "logger.h"
#include "wirelessnetwork.h"
#include "simulator.h"
#include "stats.h"

#include <cstdlib>
#include <string>

class Medium;
class Process;
class Simulator;

//Class representing a single packet in network
class Packet :
  public Process
{
public:
  //Enum used to represent Packet state
  enum class State {IN_BUFFER, MEDIUM_ACCESSING, SENDING, IN_TRANSIT, VERIFYING, RECEIVED};

  Packet(Simulator* simulator, Transmitter* transmitter);

  //on Packet destruction, signal transmitter that it can process next packet from buffer (if exists)
  ~Packet();

  void set_collision(bool collision) { collided_ = collision; }

  //Returns true if packet has no error or collision
  bool Verify() { return (!has_errors_ && !collided_); }

  int get_retransmission_count() const
  {
    return retransmission_count_;
  }

  int get_transmission_time() const
  {
    return transmission_time_;
  }

  void Execute() override;

  void Activate(int time) override;

private:
  //Pointer to simulator object, needed for agenda access etc
  Simulator* simulator_ = nullptr;

  //State of Packet process
  State state_ = State::IN_BUFFER;

  //Transmitter - origin of this packet
  Transmitter* transmitter_ = nullptr;

  int tx_id_ = -1;

  //Time needed for packet to travel between tx-rx
  int transmission_time_ = -1;

  //Flag to indicate whenever packet has errors
  bool has_errors_ = false;

  //Flag to indicate collision
  bool collided_ = false;

  //Counter to keep track of # of retransmissions
  int retransmission_count_ = -1;

  //"timestamp" used in stats
  int time_of_generation_ = -1;

  Stats* stats_ = nullptr;

  //Pointer to transmission medium used to transmit packet
  Medium* channel_ = nullptr;

  //Pointer to Logger object
  Logger* logger_ = nullptr;
};
#endif
