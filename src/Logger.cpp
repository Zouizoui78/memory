#include "Logger.hpp"

#include <iostream>
#include <time.h>

std::string getTime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [20];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer,80,"%F %T",timeinfo);
    return std::string(buffer);
}

void log(std::string level, std::string msg)
{
    std::cout << getTime() << "  [" << level << "]\t" << msg << std::endl;
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