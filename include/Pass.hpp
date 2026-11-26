#pragma once

#include <ctime>
#include <string>

struct Pass {
    std::string name;
    std::time_t start;
    std::time_t end;
    double elevation;
};
