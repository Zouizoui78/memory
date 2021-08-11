#include "Logger.hpp"

#include <iostream>
#include <fstream>
#include <ctime>

std::string getTime (const char* format = "%d-%m-%Y %H:%M:%S")
{
  time_t rawtime;
  struct tm* timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, sizeof(buffer), format, timeinfo);
  return std::string(buffer);
}

void log(std::string level, std::string msg)
{
    bool tofile = true;
    if(tofile)
    {
        std::ofstream file;
        file.open ("log.txt", std::fstream::out | std::fstream::app);
        file << getTime() << " [" << level << "] " << msg << std::endl;
        file.close();
    }
    else
        std::cout << getTime() << " [" << level << "] " << msg << std::endl;
}

void logInfo(std::string msg)
{
    #ifdef DEBUG
    log("INFO", msg);
    #else
    msg.clear();
    #endif
}

void logWarning(std::string msg)
{
    log("WARNING", msg);
}

void logError(std::string msg)
{
    log("ERROR", msg);
}

void logInit()
{
    std::remove("log.txt");
}