#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <string>
#include <iostream>

namespace Log {
    void info (std::string msg) {
        std::cout << "INFO: " << msg << std::endl;
    }

    void error (std::string msg) {
        std::cerr << "ERR: " << msg << std::endl;
    }
}

#endif