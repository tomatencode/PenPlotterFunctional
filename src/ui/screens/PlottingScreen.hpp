#pragma once

#include <functional>

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionState.hpp"
#include "jobController/JobController.hpp"
#include "jobController/JobObserver.hpp"

// Include related screens to enable navigation
#include "FilesScreen.hpp"
#include "../framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/leaves/ProgressBar.hpp"
#include "../framework/widgets/leaves/Conditional.hpp"
#include "../framework/widgets/layouts/VerticalLayout.hpp"
#include "../framework/widgets/layouts/HorizontalLayout.hpp"

namespace ui {
namespace screens {

class PlottingScreen : public Screen, public JobObserver
{
public:
    PlottingScreen(JobController& jobController,
                   MotionState& motionState,
                   std::function<bool()> wifiStatusProvider
                  )
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{.horizontalAlign = widgets::HorizontalAlignment::Center},
            widgets::make_widget<components::HeaderLine>(
                [&jobController]() {
                    String filename = jobController.getCurrentFile();
                    if (filename.endsWith(".gcode")) {
                        filename = filename.substring(0, filename.length() - 6);
                    }
                    if (filename == "") {
                        filename = "No Job";
                    }
                    return filename;
                },
                wifiStatusProvider
            ),

            widgets::make_widget<widgets::ProgressBar>(widgets::ProgressBarStyle{}, [&jobController, this]() {
                if (jobController.getTotalLines() == 0) return 0.0;
                return static_cast<double>(jobController.getCurrentLine()) / static_cast<double>(jobController.getTotalLines());
             }),

            widgets::make_widget<widgets::HorizontalLayout>(
                widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::Even},
                widgets::make_widget<widgets::Label>([&jobController, this]() {
                    return String(String(jobController.getCurrentLine()) + "/" + String(jobController.getTotalLines()));
                })
            ),

            widgets::make_widget<widgets::Conditional>(
                [&jobController]() { return jobController.isActive(); },
                widgets::make_widget<widgets::HorizontalLayout>(
                    widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::SpaceAround},

                    widgets::make_widget<widgets::Button>(
                        [&jobController, &motionState]() {
                            return motionState.getState() == MotionStateType::PAUSED ? "Resume" : "Pause";
                        },
                        widgets::ButtonStyle(),
                        [&jobController, &motionState]() {
                            if (motionState.getState() == MotionStateType::PAUSED) {
                                jobController.resume();
                            } else {
                                jobController.pause();
                            }
                        }
                    ),

                    widgets::make_widget<widgets::Button>(
                        "Abort",
                        widgets::ButtonStyle(),
                        [&jobController]() {
                            jobController.abort();
                        }
                    )
                )
            ),

            widgets::make_widget<widgets::Conditional>(
                [&jobController]() { return !jobController.isActive(); },
                widgets::make_widget<widgets::Button>(
                    "Back to Files",
                    widgets::ButtonStyle(),
                    [this, &jobController]() {
                        router()->popScreen();
                    }
                )
            )
        )
    ), _jobController(jobController)
    {
        jobController.registerObserver(this);
    }

    ~PlottingScreen() {
        _jobController.unregisterObserver(this);
    }

    void onJobEvent(const JobEventType& event) override {
        if (event.type == JobEvent::ABORTED) {
            if (router() && router()->top() == this) {  
                router()->popScreen();
            }
        }
    }

private:
    JobController& _jobController;
};

} // namespace screens
} // namespace ui