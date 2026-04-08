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
        _state.store(newState);
        if (newState == MotionStateType::IDLE && _command.load() == MotionCommand::ABORT) {
            _command.store(MotionCommand::NONE); // Clear abort command when we return to idle
        }
    }
    MotionStateType getState() const { return _state.load(); }

    void setMachineX(float x) { _machineX.store(x); }
    void setMachineY(float y) { _machineY.store(y); }
    void setPenIsDown(bool isDown) { _penIsDown.store(isDown); }

    float getMachineX() const { return _machineX.load(); }
    float getMachineY() const { return _machineY.load(); }
    bool penIsDown() const { return _penIsDown.load(); }

private:
    std::atomic<MotionCommand> _command = MotionCommand::NONE;
    std::atomic<MotionStateType> _state = MotionStateType::IDLE;

    std::atomic<float> _machineX = 0.0f;
    std::atomic<float> _machineY = 0.0f;
    std::atomic<bool> _penIsDown = false;
};