#ifndef GCodeParser_H
#define GCodeParser_H

#include "motion/MotionSystem.hpp"
#include "motion/Pen.hpp"
#include <string>
#include <vector>
#include <map>

class GCodeParser {
public:
    GCodeParser(MotionSystem& motion, Pen& pen, double feedRateDraw, double feedRateTravel)
        : _motion(motion), _pen(pen), _feedRateDraw(feedRateDraw), _feedRateTravel(feedRateTravel),
          _absolute(true) {}

    void executeLine(const std::string& line);

    void setAbsoluteMode(bool abs) { _absolute = abs; }
    void setFeedRateDraw(double f) { _feedRateDraw = f; }
    void setFeedRateTravel(double f) { _feedRateTravel = f; }

private:
    MotionSystem& _motion;
    double _feedRateDraw;
    double _feedRateTravel;
    bool _absolute;
    Pen& _pen;

    // Helpers
    void handleG0G1(const std::map<char,double>& params);
    void handleG2G3(const std::map<char,double>& params, bool clockwise);
    void handleQUAD(const std::map<char,double>& params);
    void handleCUBIC(const std::map<char,double>& params);
    void handlePenUpDown(const std::string& cmd);
};

#endif