#pragma once

#include "motion/MotionExecuter.hpp"
#include "hardware/pen/Pen.hpp"
#include "motion/HomingController.hpp"
#include "systemServices/MotionState.hpp"
#include <string>
#include <vector>
#include <map>

class GCodeExecuter {
public:
    GCodeExecuter(MotionExecuter& motion, Pen& pen, HomingController& homingController, double feedRateDraw, double feedRateTravel, MotionState& motionState);

    void executeLine(const std::string& line);
private:
    MotionExecuter& _motion;
    HomingController& _homingController;
    double _feedRateDraw;
    double _feedRateTravel;
    bool _absolute;
    Pen& _pen;
    MotionState& _motionState;

    // Helpers
    void handleG0G1(const std::map<char,double>& params);
    void handleG2G3(const std::map<char,double>& params, bool clockwise);
    void handleQUAD(const std::map<char,double>& params);
    void handleCUBIC(const std::map<char,double>& params);
    void handlePenUpDown(const std::string& cmd);
    void handleG90G91(const std::string& cmd);
    void handleHoming(const std::string& cmd);
};