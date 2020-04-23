#ifndef PROCESS_H
#define PROCESS_H

//Base class (interface) from which all objects that are processes will derive
class Process
{
public:
  virtual ~Process() = default;

  virtual void Execute() = 0;
  virtual void Activate(double time) = 0;

protected:
  bool is_terminated_ = false;
  double time_ = -1;
};



#endif