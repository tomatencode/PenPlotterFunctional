#include "GCodeParser.hpp"

GCodeParser::GCodeParser(MotionSystem& motion, Pen& pen, HomingController& homingController, double feedRateDraw, double feedRateTravel, MotionState& ms)
        : _motion(motion), _pen(pen), _homingController(homingController), _feedRateDraw(feedRateDraw), _feedRateTravel(feedRateTravel),
          _absolute(true), _ms(ms) {}

void GCodeParser::executeLine(const std::string& line) {
    std::string cmd;
    std::map<char,double> params;

    size_t i = 0;

    // Skip whitespace
    while (i < line.size() && isspace(line[i])) i++;

    // --- Capture the command token ---
    // Command can be letters followed immediately by numbers (e.g., G1, M3)
    while (i < line.size() && isalpha(line[i])) {
        cmd += toupper(line[i++]);
    }
    // Check if a number immediately follows (G1, M3, etc.)
    std::string cmdNum;
    while (i < line.size() && (isdigit(line[i]) || line[i]=='.')) {
        cmdNum += line[i++];
    }
    if (!cmdNum.empty()) cmd += cmdNum;  // e.g., "G1", "M3"

    // --- Parse remaining letters/numbers as parameters ---
    while (i < line.size()) {
        if (isalpha(line[i])) {
            char key = toupper(line[i++]);
            std::string num;
            while (i < line.size() && (isdigit(line[i]) || line[i]=='.' || line[i]=='-' || line[i]=='+')) {
                num += line[i++];
            }
            if (!num.empty()) params[key] = std::stod(num);
        } else i++;
    }

    // --- Dispatch ---
    if (cmd == "G0" || cmd == "G1") handleG0G1(params);
    else if (cmd == "G2") handleG2G3(params, true);
    else if (cmd == "G3") handleG2G3(params, false);
    else if (cmd == "G5") handleQUAD(params);
    else if (cmd == "G5.1") handleCUBIC(params);
    else if (cmd == "M3" || cmd == "M5") handlePenUpDown(cmd);
    else if (cmd == "G90" || cmd == "G91") handleG90G91(cmd);
    else if (cmd == "G28") handlehoming(cmd);
    else {
        Serial.print("Unknown command: ");
        Serial.println(cmd.c_str());
    }
}

void GCodeParser::handleG0G1(const std::map<char,double>& params) {
    XYPos target = _motion.getCurrentPos();
    double feed = _pen.isDown() ? _feedRateDraw : _feedRateTravel;

    if (params.count('X')) target.x_mm = _absolute ? params.at('X') : target.x_mm + params.at('X');
    if (params.count('Y')) target.y_mm = _absolute ? params.at('Y') : target.y_mm + params.at('Y');
    if (params.count('F')) feed = params.at('F');

    _motion.moveToXY(target, feed);
}

void GCodeParser::handleG2G3(const std::map<char,double>& params, bool clockwise) {
    XYPos current = _motion.getCurrentPos();
    if (!params.count('X') || !params.count('Y') || !params.count('I') || !params.count('J')) return;

    XYPos center = {current.x_mm + params.at('I'), current.y_mm + params.at('J')};
    XYPos target;
    target.x_mm = _absolute ? params.at('X') : current.x_mm + params.at('X');
    target.y_mm = _absolute ? params.at('Y') : current.y_mm + params.at('Y');
    double feed = _pen.isDown() ? _feedRateDraw : _feedRateTravel;
    if (params.count('F')) feed = params.at('F');

    _motion.arcToXY(target, center, clockwise, feed);
}

void GCodeParser::handleQUAD(const std::map<char,double>& params) {
    XYPos current = _motion.getCurrentPos();
    if (!params.count('X') || !params.count('Y') || !params.count('C') || !params.count('D')) return;

    XYPos target, control;
    target.x_mm = _absolute ? params.at('X') : current.x_mm + params.at('X');
    target.y_mm = _absolute ? params.at('Y') : current.y_mm + params.at('Y');
    control.x_mm = params.at('C');       // Control X
    control.y_mm = params.at('D');       // Control Y (use 'D' as second letter for clarity)
    double feed = _pen.isDown() ? _feedRateDraw : _feedRateTravel;
    if (params.count('F')) feed = params.at('F');

    _motion.quadraticBezierToXY(control, target, feed);
}

void GCodeParser::handleCUBIC(const std::map<char,double>& params) {
    XYPos current = _motion.getCurrentPos();
    if (!params.count('X') || !params.count('Y') || !params.count('A') || !params.count('B') || !params.count('C') || !params.count('D')) return;

    XYPos target, c1, c2;
    target.x_mm = _absolute ? params.at('X') : current.x_mm + params.at('X');
    target.y_mm = _absolute ? params.at('Y') : current.y_mm + params.at('Y');
    c1.x_mm = params.at('A'); c1.y_mm = params.at('B');
    c2.x_mm = params.at('C'); c2.y_mm = params.at('D');
    double feed = _pen.isDown() ? _feedRateDraw : _feedRateTravel;
    if (params.count('F')) feed = params.at('F');

    _motion.cubicBezierToXY(c1, c2, target, feed);
}

// Pen up/down
void GCodeParser::handlePenUpDown(const std::string& cmd) {
    if (cmd == "M3") {
        _pen.down();
    } else if (cmd == "M5") {
        _pen.up();
    }
}

void GCodeParser::handleG90G91(const std::string& cmd) {
    if (cmd == "G90") {
        _absolute = true;
    } else if (cmd == "G91") {
        _absolute = false;
    }
}

void GCodeParser::handlehoming(const std::string& cmd) {
    if (cmd == "G28") {
        _homingController.home();
    }
}