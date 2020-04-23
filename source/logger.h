#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#undef ERROR //name conflict with Mode::ERROR enum

//TODO Add some way to format messages
/*Class responsible for logging information to specified file and/or console.
  Console - Windows OS specific.
 */
class Logger
{
//***************************************PUBLIC***************************************
public:
  //====================TYPES DECLARATION====================

    //Flags to set which messages will be logged
  enum class Mode { ERROR = 0x01, DEBUG = 0x02, INFO = 0x04 };

    //Flags to specify where to direct logged messages
  enum class Target { CONSOLE = 0x01, FILE = 0x02 };

  //====================CONSTANTS/TYPEDEFS====================

  //====================CONSTRUCTORS/DESTRUCTOR====================

  explicit Logger(const std::string file_path);
  ~Logger();

  //====================METHODS====================
  
    //Sets log-mode flag
  void set_mode_flag(Mode mode)
  {
    mode_ |= static_cast<unsigned char>(mode);
  }

    //Clears log-mode flag
  void clear_mode_flag(Mode mode)
  {
    mode_ &= !static_cast<unsigned char>(mode);
  }

    //Activates logging to target
  void set_target_flag(Target target)
  {
    target_ |= static_cast<unsigned char>(target);
  }
    //Deactivates logging to target
  void clear_target_flag(Target target)
  {
    target_ &= !static_cast<unsigned char>(target);
  }

  //====================FIELDS====================

    //Logs message if ERROR mode flag is set
  void Error(const std::string &message);
    //Logs message if DEBUG mode flag is set
  void Debug(const std::string &message);
    //Logs message if INFO mode flag is set
  void Info(const std::string &message);

//***************************************PRIVATE***************************************
private:
  //====================METHODS====================

  bool is_bit_set(const unsigned char bitfield, Mode mode)
  {
    return bitfield & static_cast<unsigned char>(mode);
  }

  bool is_bit_set(const unsigned char bitfield, Target target)
  {
    return bitfield & static_cast<unsigned char>(target);
  }

    //Appends message without new line to file specified by file_path_
  bool LogToFile(const std::string& message);

    //Logs message to console with specified text color
  void LogToConsole(const std::string& message, int text_color);

  //====================FIELDS====================

    //Variable to keep mode setting (bitwise)
  unsigned char mode_ = 0;
    //Variable to keep target setting (bitwise)
  unsigned char target_ = 0;

  //bool time_stamp_;

  std::string file_path_;
  std::ofstream out_file_;

  HANDLE h_console_;
  int console_default_color_;
};



#endif
