#pragma once

#include <functional>

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionState.hpp"
#include "jobController/JobController.hpp"

namespace ui {
namespace screens {

class PlottingScreen : public ui::Screen, public JobObserver
{
public:
    PlottingScreen(JobController& jobController, MotionState& motionState, std::function<bool()> wifiStatusProvider);

    void onJobEvent(const JobEventType& event) override;
};

} // namespace screens
} // namespace ui