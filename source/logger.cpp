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

#ifdef INDENT_ENABLE
void Logger::IndentForward()
{
  //for now, just hope that noone will call this more than 127 times in a row
  ++ind_level_;
}

void Logger::IndentBack()
{
  --ind_level_;
}
#endif

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

/*TODO change this to writing to some buffor and then write buffor to file, absence of buffor results in
  pretty unhealthy amount of file opening and closing beneath*/
bool Logger::LogToFile(const std::string& message)
{
  //TODO: some checking if writing was successful
  out_file_.open(file_path_, std::ofstream::out | std::ofstream::app);

  if(!out_file_.is_open())
    return false;

#ifdef INDENT_ENABLE
  //clip
  char ind = (ind_level_ > MAX_INDENT_LEVEL) ? MAX_INDENT_LEVEL : ind_level_;
  ind = (ind <= 0) ? 0 : ind;
  //TODO change this to some more sophisticated method
  for(int i = 0; i < ind; ++i)
    out_file_ << '\t';
#endif

  out_file_ << message;
  out_file_.close();
  return false;
}

void Logger::LogToConsole(const std::string& message, int text_color)
{
  
#ifdef INDENT_ENABLE
  //clip
  char ind = (ind_level_ > MAX_INDENT_LEVEL) ? MAX_INDENT_LEVEL : ind_level_;
  ind = (ind <= 0) ? 0 : ind;
  //TODO change this to some more sophisticated method
  for(int i = 0; i < ind; ++i)
    std::cout << '\t';
#endif

  SetConsoleTextAttribute(h_console_, text_color);
  std::cout << message;
  SetConsoleTextAttribute(h_console_, console_default_color_);
}



