#include "GCodeParser.hpp"

std::optional<GCodeCommand> GCodeParser::parse(const std::string& line)
{
    GCodeCommand result;
    size_t i = 0;

    // Skip leading whitespace
    while (i < line.size() && isspace(line[i])) i++;

    // Skip comment-only lines (';' or '(')
    if (i >= line.size() || line[i] == ';' || line[i] == '(') return std::nullopt;

    // Capture command letter(s) (e.g., 'G', 'M')
    while (i < line.size() && isalpha(line[i])) {
        result.cmd += toupper(line[i++]);
    }

    // Capture command number, including decimals (e.g., "1", "5.1")
    std::string cmdNum;
    while (i < line.size() && (isdigit(line[i]) || line[i] == '.')) {
        cmdNum += line[i++];
    }
    if (!cmdNum.empty()) result.cmd += cmdNum;

    if (result.cmd.empty()) return std::nullopt;

    // Parse parameter letters and their values
    while (i < line.size()) {
        if (line[i] == ';') break; // inline comment — stop parsing
        if (isalpha(line[i])) {
            char key = toupper(line[i++]);
            std::string num;
            while (i < line.size() && (isdigit(line[i]) || line[i] == '.' || line[i] == '-' || line[i] == '+')) {
                num += line[i++];
            }
            if (!num.empty()) result.params[key] = std::stod(num);
        } else {
            i++;
        }
    }

    return result;
}
