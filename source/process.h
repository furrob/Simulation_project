#ifndef PROCESS_H
#define PROCESS_H

class Simulator;

//Base class (interface) from which all objects that are processes will derive
class Process
{
public:
  virtual ~Process() = default; //pretty important

  virtual void Execute() = 0;
  virtual void Activate(int time) = 0;

  int get_time() const
  {
    return time_;
  }

  int get_id() const
  {
    return id_;
  }

  bool is_terminated() const
  {
    return is_terminated_;
  }

  bool is_last() const
  {
    return last_;
  }

protected:
  int id_ = 0;
  bool is_terminated_ = false;
  bool last_ = false;
  int time_ = -1;
  Simulator* simulator_ = nullptr;
};



#endif