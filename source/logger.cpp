#include "logger.h"

Logger::Logger(const std::string file_path): file_path_(file_path)
{
  h_console_ = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_SCREEN_BUFFER_INFO console_info;
  GetConsoleScreenBufferInfo(h_console_, &console_info);
  console_default_color_ = console_info.wAttributes;
}

Logger::~Logger()
{

}

void Logger::Error(const std::string& message)
{
  if(is_bit_set(mode_, Mode::ERROR))
  {
    if(is_bit_set(target_, Target::CONSOLE))
      LogToConsole(message, BACKGROUND_RED);

    if(is_bit_set(target_, Target::FILE))
      LogToFile(message);
  }
}

void Logger::Debug(const std::string& message)
{
  if(is_bit_set(mode_, Mode::DEBUG))
  {
    if(is_bit_set(target_, Target::CONSOLE))
      LogToConsole(message, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    if(is_bit_set(target_, Target::FILE))
      LogToFile(message);
  }
}

void Logger::Info(const std::string& message)
{
  if(is_bit_set(mode_, Mode::INFO))
  {
    if(is_bit_set(target_, Target::CONSOLE))
      LogToConsole(message, FOREGROUND_INTENSITY);

    if(is_bit_set(target_, Target::FILE))
      LogToFile(message);
  }
}

bool Logger::LogToFile(const std::string& message)
{
  //TODO: some checking if writing was successful
  out_file_.open(file_path_, std::ofstream::out | std::ofstream::app);

  if(!out_file_.is_open())
    return false;
  
  out_file_ << message;
  out_file_.close();
  return false;
}

void Logger::LogToConsole(const std::string& message, int text_color)
{
  SetConsoleTextAttribute(h_console_, text_color);
  std::cout << message;
  SetConsoleTextAttribute(h_console_, console_default_color_);
}



