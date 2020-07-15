#include "inc.h"

#define NO_SEEDS 31

//parameters: seeds, lambda, WirelessNetwork::P

int main(int argc, char* argv[])
{
 #pragma region SeedGeneration
    /*
    std::ofstream out_file("./seeds.txt");
    std::vector<int> out_seeds;

    RandomGenerator rnd(135580);

    for(int i = 0; i < 15 * NO_SEEDS; ++i) //15 simulations * 31 different seeds * 2000000 seed spacing
    {
      for(int j=0; j<200000; ++j)
      {
        rnd.NextSeed();
      }
      out_seeds.push_back(rnd.NextSeed());
    }

    std::ostream_iterator<int> output_iterator(out_file, "\n");
    std::copy(out_seeds.begin(), out_seeds.end(), output_iterator);
    */
#pragma endregion
  std::cout << "M4 A3\n\n";

#ifdef _DEBUG
  std::cout << "DEBUG\n\n\n";
#endif

  std::cout << "Number of simulations <1 - 15>: ";
  int sim_count;
  std::cin >> sim_count;
  std::cout << std::endl;

  std::cout << "Max retransmissions: ";
  int max_retransmission_count = 5;
  std::cin >> max_retransmission_count;
  std::cout << std::endl;

  std::cout << "Lambda: ";
  double lambda;
  std::cin >> lambda;
  std::cout << std::endl;

  std::cout << "Max simulation clock [ms]: ";
  double max_clock;
  std::cin >> max_clock;
  std::cout << std::endl;

  std::cout << "Initial phase duration [ms]: ";
  double initial_phase;
  std::cin >> initial_phase;
  std::cout << std::endl;

  for(int i = 1; i <= sim_count; ++i)
  {
    int seeds_set = i;

    //PREPARE DATA
      //all seeds in one file, 15 sets 31 seeds each
    std::ifstream in_file("./seeds.txt");

    std::vector<int> seeds;
    std::string file_line;

    for(int i = 0; i < (seeds_set - 1) * NO_SEEDS; ++i) //skip some seeds based on which set is chosen
      std::getline(in_file, file_line);

    for(int i = 0; i < NO_SEEDS; ++i) //assumed there is enough seeds
    {
      std::getline(in_file, file_line);
      seeds.push_back(std::stoi(file_line));
    }

    if(seeds.size() < NO_SEEDS)
    {
      std::cout << "NOT ENOUGH SEEDS\n";
      _getch();//some pause !NOT _getch()!
      return -1;
    }

    //PREPARE AND RUN
    std::string log_file = "./LOGS/LOG_LAM" + std::to_string(static_cast<int>(lambda * 1000000)) + ".txt";
    Logger logger(log_file);
    logger.set_flag(Logger::Target::CONSOLE);

    logger.set_flag(Logger::Mode::INFO);
    logger.set_flag(Logger::Mode::ERROR);
#ifdef _DEBUG
    logger.set_flag(Logger::Mode::DEBUG);
#endif

    auto stats = Stats(WirelessNetwork::terminal_pairs_count, lambda);

    Simulator simulator = Simulator(&logger, &stats, lambda);
    simulator.Init(&seeds, max_retransmission_count); //add lambda and faza pocz¹tkowa as input parameters

    simulator.Run(max_clock, initial_phase); //in milliseconds

#pragma region InitialPhaseData
  ////dump initial phase data to file
  //std::string file_name = "./SIM_s" + std::to_string(seeds_set) + "_l" + std::to_string(lambda) + ".txt";
  //std::ofstream out_file(file_name);

  //std::ostream_iterator<double> os_it(out_file, "\n");
  //std::copy(stats.avg_retransmissions_.begin(), stats.avg_retransmissions_.end(), os_it);
#pragma endregion

    logger.set_flag(Logger::Target::FILE);
    //view collected data and simulation parameters
    logger.Info("==========STATS==========\n\n");

    logger.Info("Seeds set:\t\t\t " + std::to_string(seeds_set) + "\n" +
      "Maximum packet retransmissions:\t " + std::to_string(max_retransmission_count) + "\n" +
      "Lambda value:\t\t\t " + std::to_string(lambda) + "\n" +
      "Simulation length:\t\t " + std::to_string(static_cast<int>(max_clock - initial_phase)) + " [ms]\n\n");

    logger.Info("Packets generated (buff. incl.): " + std::to_string(stats.PacketsGenerated()) + "\n" +
      "Packets successfully received:\t " + std::to_string(stats.PacketsReceived()) + "\n" +
      "Packets lost:\t\t\t " + std::to_string(stats.PacketsLost()) + "\n\n");

    logger.Info("AVG Packet Error Rate:\t\t " + std::to_string(stats.AvgPacketErrorRate()) + "\n" +
      "Max Packet Error Rate:\t\t " + std::to_string(stats.MaxPacketErrorRate()) + "\n" +
      "AVG Retransmission per Packet:\t " + std::to_string(stats.AvgRetransmissionsCount()) + "\n" +
      "Network Packet throughput:\t " + std::to_string(stats.PacketThroughput()) + " packets/s\n" +
      "AVG Packet Delay:\t\t " + std::to_string(stats.AvgPacketDelay()) + " [ms]\n" +
      "AVG Packet Buff Wait Time:\t " + std::to_string(stats.AvgPacketWaitingTime()) + " [ms]\n\n\n");
  }
  _getch();

#pragma region Histograms
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
#pragma endregion

  return 0;
}


