#include "randomgenerator.h"

RandomGenerator::RandomGenerator(int seed, double lambda)
{
  seed_ = seed;
  lambda_ = lambda;
}

int RandomGenerator::NextSeed()
{
  const int h = static_cast<int>(floor(seed_ / Q));
  seed_ = A * (seed_ - Q * h) - R * h;
  seed_ = (seed_ < 0) ? static_cast<int>(seed_ + M) : seed_;
  return seed_;
}

double RandomGenerator::Rand()
{
  return NextSeed() / M;
}

double RandomGenerator::Rand(double min, double max)
{
  return Rand() * (max - min) + min;
}

double RandomGenerator::RandExp()
{
  double k = Rand();
  return -(1.0 / lambda_) * log(k);
}

bool RandomGenerator::RandBin(double p)
{
  return (Rand() >= p) ? false : true;
}
