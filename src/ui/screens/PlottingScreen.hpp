#pragma once

#include <functional>

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionState.hpp"
#include "jobController/JobController.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class PlottingScreen : public ui::Screen, public JobObserver
{
public:
    PlottingScreen(const String& filename, JobController& jobController, MotionState& motionState, std::function<bool()> wifiStatusProvider);

    void onJobEvent(const JobEventType& event) override;
private:
    const String _filename;
    JobController& _jobController;
    
    bool _completed = false;
    bool _jobOutdated = false;
};

} // namespace screens
} // namespace ui