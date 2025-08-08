#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

// Global flag to control logging.
extern bool g_debug_logging_enabled;

#define LOG_DEBUG(x) \
    do { \
        if (g_debug_logging_enabled) { \
            std::cerr << x << std::endl; \
        } \
    } while (0)

#endif // LOGGER_HPP
