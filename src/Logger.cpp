#include "Logger.hpp"

#include <iostream>
#include <fstream>
#include <date.h>


void log(std::string level, std::string msg)
{
    using namespace date;
    bool tofile = true;
    if(tofile)
    {
        std::ofstream file;
        file.open ("log.txt", std::fstream::out | std::fstream::app);
        file << std::chrono::system_clock::now() << " [" << level << "] " << msg << std::endl;
        file.close();
    }
    else
        std::cout << std::chrono::system_clock::now() << " [" << level << "] " << msg << std::endl;
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