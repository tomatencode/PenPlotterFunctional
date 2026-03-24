#pragma once

#include <atomic>

enum class MotionCommand
{
    NONE = 0,
    PAUSE = 1,
    ABORT = 2
};

enum class MotionState
{
    IDLE = 0,
    RUNNING = 1,
    PAUSED = 2
};

class MotionStateManager {
public:
    void setCommand(MotionCommand cmd) { command.store(cmd); }
    MotionCommand getCommand() const { return command.load(); }

    void setPosition(float x, float y) {
        machineX.store(x);
        machineY.store(y);
    }
    void setState(MotionState newState) {
        state.store(newState);
    }
    std::pair<float, float> getMachinePos() const { return {machineX.load(), machineY.load()}; }
    MotionState getState() const { return state.load(); }

private:
    std::atomic<MotionCommand> command = MotionCommand::NONE;
    std::atomic<float> machineX = 0.0f;
    std::atomic<float> machineY = 0.0f;
    std::atomic<MotionState> state = MotionState::IDLE;
};