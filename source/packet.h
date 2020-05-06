#ifndef PACKET_H
#define PACKET_H

#include "process.h"
#include "transmitter.h"
#include "medium.h"
#include "logger.h"
#include "wirelessnetwork.h"
#include "simulator.h"

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
  enum class State {IN_BUFFER, MEDIUM_ACCESSING, IN_TRANSIT, VERIFYING, RECEIVED};

  Packet(Simulator* simulator, Transmitter* transmitter);

  //on Packet destruction, signal transmitter that it can process next packet from buffer (if exists)
  ~Packet();

  //Returns random retransmission time for specified packet
  double GetCRPTime();

  void set_collision(bool collision) { collided_ = collision; }

  //Randomly marks Packet as corrupted (has_errors_ = true)
  void TransmissionError(int percent = 30) { has_errors_ = ((rand() % 100 + 1) <= percent) ? true : false; } //TEST

  //Returns true if packet has no error or collision
  bool Verify() { return (!has_errors_ && !collided_); }

  //TEST TEST TEST TEST TEST
  bool terminated() { return is_terminated_; }

  void Execute() override;

  void Activate(double time) override;

private:
  //Pointer to simulator object, needed for agenda access etc
  Simulator* simulator_ = nullptr;

  //State of Packet process
  State state_ = State::IN_BUFFER;

  //Transmitter - origin of this packet
  Transmitter* transmitter_ = nullptr;

  //Time needed for packet to travel between tx-rx
  double transmission_time_ = 0;

  //Flag to indicate whenever packet has errors
  bool has_errors_ = false;

  //Flag to indicate collision
  bool collided_ = false;

  //Flag used to chceck if packet was transmitted correctly
  bool ack_ = false;

  //Counter to keep track of # of retransmissions
  int retransmission_count_ = 0;

  //Pointer to transmission medium used to transmit packet
  Medium* channel_ = nullptr;

  //Pointer to Logger object
  Logger* logger_ = nullptr;
};
#endif
