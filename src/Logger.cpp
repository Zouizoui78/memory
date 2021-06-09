#include "Logger.hpp"

#include <iostream>
#include <fstream>
#include <date.h>


void log(std::string level, std::string msg)
{
    using namespace date;
    std::ofstream file;
    file.open ("log.txt", std::fstream::out | std::fstream::app);
    file << std::chrono::system_clock::now() << " [" << level << "] " << msg << std::endl;
    file.close();
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

void logInit()
{
    std::remove("log.txt");
}