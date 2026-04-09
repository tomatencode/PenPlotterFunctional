#pragma once

#include <optional>
#include <string>
#include "GCodeCommand.hpp"

class GCodeParser {
public:
    // Returns nullopt if the line is empty or comment-only
    static std::optional<GCodeCommand> parse(const std::string& line);
};
