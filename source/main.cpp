#include "inc.h"

int main(int argc, char* argv[])
{
  //TODO maybe add some parameter to then save and use globally in seeds (multiply by it or something) to add degree of freedom
  //TODO and lambda
  std::cout << "M4 A3\n";
  
  Logger logger("./../LOG.txt");
  logger.set_flag(Logger::Target::CONSOLE);
  //logger.set_flag(Logger::Target::FILE); <- not quite ready for now (it works, but not humanitarian enough)

  logger.set_flag(Logger::Mode::DEBUG);

  Simulator simulator = Simulator(&logger, 0.01);
  simulator.Init(); //add lambda and faza pocz¹tkowa as input parameters

  logger.set_flag(Logger::Mode::ERROR);
  logger.set_flag(Logger::Mode::INFO);
  logger.set_flag(Logger::Mode::DEBUG);


  simulator.Run(5000); //100ms limit results in ~1000 main loop iterations; 500ms - around 10k

  return 0;
}

/*
   int number_of_rands = 100000;
  auto numbers = std::vector<double>();
  {    //***UNIFORM***
    int uniform_seed = 113603;
    auto uniform_generator = RandomGenerator(uniform_seed);

    for(int i = 0; i < number_of_rands; ++i)
      numbers.push_back(uniform_generator.Rand());

    std::ofstream output_file("./hist/uniform.txt");
    std::ostream_iterator<double> output_iterator(output_file, "\n");
    std::copy(numbers.begin(), numbers.end(), output_iterator);

    numbers.clear();
  }
  {    //***BINARY 25%***
    int binary_seed = 774298;
    auto binary_generator = RandomGenerator(binary_seed);

    for(int i = 0; i < number_of_rands; ++i)
      numbers.push_back(binary_generator.RandBin(0.25));

    std::ofstream output_file("./hist/binary25.txt");
    std::ostream_iterator<double> output_iterator(output_file, "\n");
    std::copy(numbers.begin(), numbers.end(), output_iterator);

    numbers.clear();
  }
  {    //***BINARY 50%***
    int binary_seed = 794870;
    auto binary_generator = RandomGenerator(binary_seed);

    for(int i = 0; i < number_of_rands; ++i)
      numbers.push_back(binary_generator.RandBin(0.50));

    std::ofstream output_file("./hist/binary50.txt");
    std::ostream_iterator<double> output_iterator(output_file, "\n");
    std::copy(numbers.begin(), numbers.end(), output_iterator);

    numbers.clear();
  }
  {    //***BINARY 75%***
    int binary_seed = 785611;
    auto binary_generator = RandomGenerator(binary_seed);

    for(int i = 0; i < number_of_rands; ++i)
      numbers.push_back(binary_generator.RandBin(0.75));

    std::ofstream output_file("./hist/binary75.txt");
    std::ostream_iterator<double> output_iterator(output_file, "\n");
    std::copy(numbers.begin(), numbers.end(), output_iterator);

    numbers.clear();
  }
  {    //***EXPONENTIAL LAMBDA 1***
    int exp_seed = 683354;
    auto exp_generator = RandomGenerator(exp_seed, 1);

    for(int i = 0; i < number_of_rands; ++i)
      numbers.push_back(exp_generator.RandExp());

    std::ofstream output_file("./hist/exponential1.txt");
    std::ostream_iterator<double> output_iterator(output_file, "\n");
    std::copy(numbers.begin(), numbers.end(), output_iterator);

    numbers.clear();
  }
  {    //***EXPONENTIAL LAMBDA 0.1***
    int exp_seed = 590083;
    auto exp_generator = RandomGenerator(exp_seed, 0.1);

    for(int i = 0; i < number_of_rands; ++i)
      numbers.push_back(exp_generator.RandExp());

    std::ofstream output_file("./hist/exponential01.txt");
    std::ostream_iterator<double> output_iterator(output_file, "\n");
    std::copy(numbers.begin(), numbers.end(), output_iterator);

    numbers.clear();
  }
  {    //***EXPONENTIAL LAMBDA 0.01***
    int exp_seed = 356821;
    auto exp_generator = RandomGenerator(exp_seed, 0.01);

    for(int i = 0; i < number_of_rands; ++i)
      numbers.push_back(exp_generator.RandExp());

    std::ofstream output_file("./hist/exponential001.txt");
    std::ostream_iterator<double> output_iterator(output_file, "\n");
    std::copy(numbers.begin(), numbers.end(), output_iterator);

    numbers.clear();
  }
 
 */
