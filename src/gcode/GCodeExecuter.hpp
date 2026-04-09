#pragma once

#include "motionExecution/MotionExecuter.hpp"
#include "pen/Pen.hpp"
#include "homing/HomingController.hpp"
#include "rtos/MotionState.hpp"
#include "settings/RuntimeSettings.hpp"
#include "GCodeCommand.hpp"
#include <string>
#include <map>

class GCodeExecuter {
public:
    GCodeExecuter(MotionExecuter& motion, Pen& pen, HomingController& homingController, 
                  RuntimeSettings& runtimeSettings, MotionState& motionState);

    void execute(const GCodeCommand& command);
private:
    Pen& _pen;
    MotionExecuter& _motion;
    HomingController& _homingController;
    RuntimeSettings& _runtimeSettings;
    MotionState& _motionState;
    bool _absolute;

    // Helpers
    void handleG0G1(const std::map<char,double>& params);
    void handleG2G3(const std::map<char,double>& params, bool clockwise);
    void handleQUAD(const std::map<char,double>& params);
    void handleCUBIC(const std::map<char,double>& params);
    void handlePenUpDown(const std::string& cmd);
    void handleG90G91(const std::string& cmd);
    void handleHoming(const std::string& cmd);
};