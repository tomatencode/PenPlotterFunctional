#ifndef GCodeParser_H
#define GCodeParser_H

#include "motion/MotionSystem.hpp"
#include "motion/Pen.hpp"
#include "motion/HomingController.hpp"
#include <string>
#include <vector>
#include <map>

class GCodeParser {
public:
    GCodeParser(MotionSystem& motion, Pen& pen, HomingController& homingController, double feedRateDraw, double feedRateTravel);

    void executeLine(const std::string& line);
private:
    MotionSystem& _motion;
    HomingController& _homingController;
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
    void handleG90G91(const std::string& cmd);
    void handlehoming(const std::string& cmd);
};

#endif