#pragma once

#include <string>
#include <map>

struct GCodeCommand {
    std::string cmd;
    std::map<char, double> params;
};
