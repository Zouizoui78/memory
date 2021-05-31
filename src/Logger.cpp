#include "Logger.hpp"

#include <iostream>
#include <date/date.h>


void log(std::string level, std::string msg)
{
    using namespace date;
    std::cout << std::chrono::system_clock::now() << " [" << level << "] " << msg << std::endl;
}

void logInfo(std::string msg)
{
    log("INFO", msg);
}

void logWarning(std::string msg)
{
    log("WARNING", msg);
}

void logError(std::string msg)
{
    log("ERROR", msg);
}