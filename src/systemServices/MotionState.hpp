#pragma once

#include <atomic>

enum class MotionCommand
{
    NONE = 0,
    PAUSE = 1,
    ABORT = 2
};

enum class MotionStateType
{
    IDLE = 0,
    RUNNING = 1,
    PAUSED = 2
};

class MotionState {
public:
    void setCommand(MotionCommand cmd) { _command.store(cmd); }
    MotionCommand getCommand() const { return _command.load(); }

    void setState(MotionStateType newState) {
        state.store(newState);
        if (newState == MotionStateType::IDLE && _command.load() == MotionCommand::ABORT) {
            _command.store(MotionCommand::NONE); // Clear abort command when we return to idle
        }
    }
    MotionStateType getState() const { return state.load(); }

    void setMachineX(float x) { _machineX.store(x); }
    void setMachineY(float y) { _machineY.store(y); }

    float getMachineX() const { return _machineX.load(); }
    float getMachineY() const { return _machineY.load(); }

private:
    std::atomic<MotionCommand> _command = MotionCommand::NONE;
    std::atomic<float> _machineX = 0.0f;
    std::atomic<float> _machineY = 0.0f;
    std::atomic<MotionStateType> state = MotionStateType::IDLE;
};