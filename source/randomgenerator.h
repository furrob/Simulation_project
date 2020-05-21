#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <cmath>

class RandomGenerator
{
public:
  RandomGenerator(int seed, double lambda = 1);

  //Used for generating random seeds based on "alpha seed" from constructor
  int NextSeed();

  //Draws a number between <0;1>
  double Rand();

  //Draws a number between <min;max>
  double Rand(double min, double max);

  //Returns number with exponential distribution (intensity lambda, specified in constructor)
  double RandExp();

  //Returns true or false based on probability p (p - true; 1-p - false)
  bool RandBin(double p);

private:
  static constexpr double M = 2147483647.0;
  static constexpr int A = 16807;
  static constexpr int Q = 127773;
  static constexpr int R = 2836;

  int seed_;
  double lambda_;
};


#endif