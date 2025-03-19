#pragma once

#include <stdexcept>
#include <string>
struct parsing_error : std::logic_error {
    parsing_error(std::string message)
        : std::logic_error(message)
    {}
};