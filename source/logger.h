#ifndef LOGGER_H
#define LOGGER_H

//If uncommented, enables indentation functionality
#define INDENT_ENABLE
//^just for fun

//Maximum level of nested \t, every further incrementation try
//won't take any visual effect - to avoid messing with previous tabulators,
//overflow counter will be incremented
#define MAX_INDENT_LEVEL 5

#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#undef ERROR //name conflict with Mode::ERROR enum

//TODO Add some way to format messages - indentation levels, "appending" messages to actual line (bypassing indentation level for some messages
//todo  to avoid weird tabs between)

/*Fairly simple class responsible for logging information to specified file and/or console.
Console - Windows OS specific.

Sample usage for displaying only Debug messages to both console and file:
  Logger logger("LOG.txt");  //specify path to log file
  logger.set_flag(LOGGER::TARGET::FILE); //enable logging to file - after creation all options are disabled
  logger.set_flag(LOGGER::TARGET::CONSOLE);
  logger.set_flag(LOGGER::MODE::Debug);  //enable Debug type messages - .Info() or .Error() calls
                                         //ale valid, but wont take any effect
  logger.Debug("Sample debug message\n");  //newline character is not added automatically
  logger.IndentForward(); //makes new paragraph to help to differentiate messages from different places.
                          //'\t' is added no matter that previous message contained '\n' or not
                          //ONLY IF INDENT_ENABLE IN logger.h IS DEFINED
  foo(); //every message printed from foo() will be indented
  logger.IndentBack(); //end paragraph
  logger.Info("Info message that won't be logged"); //to log that message, call set_flag(LOGGER::MODE::INFO)

Flags (target or mode type) can be freely manipulated within further program code
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
#ifdef INDENT_ENABLE
    //Adds one '\t' char for every call. Tabulation level is clipped to
    //MAX_INDENDT_LEVEL specified in logger.h file.
    //Although further calls wont take any visual effect, nonetheless internal counter will
    //be incremented - thus IndentBack() may not have visible effect.
    //To keep formatting stable, every IndentForward() call must be
    //followed by IndentBack() call.
  void IndentForward();

    //Removes one '\t' char for every call. Minimum is 0 -
    //further calls will be saved, in this situation IndentForward()
    //calls may take no effect until internal counter will again reach 0.
    //Rule is simple - for every IndentBack() must exist exactly one
    //IndentForward() before it.
  void IndentBack();
#else
    //Method inactive, to enable define INDENT_ENABLE in logger.h
  void IndentForward(){}
    //Method inactive, to enable define INDENT_ENABLE in logger.h
  void IndentBack(){}
#endif

    //Sets log-mode flag
  void set_flag(Mode mode)
  {
    mode_ |= static_cast<unsigned char>(mode);
  }

    //Clears log-mode flag
  void clear_flag(Mode mode)
  {
    mode_ &= !static_cast<unsigned char>(mode);
  }

    //Activates logging to target
  void set_flag(Target target)
  {
    target_ |= static_cast<unsigned char>(target);
  }
    //Deactivates logging to target
  void clear_flag(Target target)
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

#ifdef INDENT_ENABLE
    //Specifies how many tabs should be inserted before displaying message,
    //clipped to MAX_INDENT_LEVEL, further calls to IndentForward() will
    //increment this counter, but will take no visual effects
    //Similarly with calls on IndentBack() - this counter can go negative,
    //but minimal level is 0
  char ind_level_ = 0;
#endif

  //bool time_stamp_;

  std::string file_path_;
  std::ofstream out_file_;

  HANDLE h_console_;
  int console_default_color_;
};



#endif
