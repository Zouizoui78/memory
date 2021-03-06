#ifndef LOGGER
#define LOGGER

#include <string>

/**
 * Info level logging.
 * 
 * @param msg Text to log.
 */
void logInfo(std::string msg);

/**
 * Warning level logging.
 * 
 * @param msg Text to log.
 */
void logWarning(std::string msg);

/**
 * Error level logging.
 * 
 * @param msg Text to log.
 */
void logError(std::string msg);

/**
 * Base logging function.
 * 
 * @param level Logging level.
 * @param msg Text to log.
 */
void log(std::string level, std::string msg);

/**
 * @brief Remove log file if already present.
 * 
 */
void logInit();

#endif // LOGGER