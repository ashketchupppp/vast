#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <string>
#include <iostream>

// Logging methods are inlined for performance reasons

namespace Log {
    inline void info (std::string msg) {
        std::cout << "INFO: " << msg << std::endl;
    }

    inline void error (std::string msg) {
        std::cerr << "ERR: " << msg << std::endl;
    }

    inline void perf (std::string msg, bool endl = false) {
        std::cout << " PERF: " << msg;
        if (endl) std::cout << std::endl;
    }
}

#endif