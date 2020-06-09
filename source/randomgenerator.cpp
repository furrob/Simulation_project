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

int RandomGenerator::Rand(int min, int max)
{
  double min_ = min / 10;
  double max_ = max / 10;
  return 10 * static_cast<int>(floor(Rand() * (max_ - min_) + min_)); //fast and probably ugly fix to retrieve "integer" values of milliseconds (10, 20 and so on)
}

int RandomGenerator::RandExp()
{
  double k = Rand();
  return 10*(static_cast<int>(-(1.0 / lambda_) * log(k))/10); //return static_cast<int>(-(1.0 / lambda_) * log(k));
}

bool RandomGenerator::RandBin(double p)
{
  return (Rand() >= p) ? false : true;
}
