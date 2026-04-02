#pragma once

#include <functional>
#include <map>

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionState.hpp"
#include "jobController/JobController.hpp"
#include "jobController/JobObserver.hpp"

// Include related screens to enable navigation
#include "FilesScreen.hpp"
#include "../framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/leaves/ProgressBar.hpp"
#include "../framework/widgets/leaves/Switch.hpp"

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
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical, .horizontalAlign = widgets::HorizontalAlignment::Center},
            std::make_unique<components::HeaderLine>(
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

            std::make_unique<widgets::ProgressBar>(widgets::ProgressBarProps{
                .getProgress = [&jobController, this]() {
                    if (jobController.getTotalLines() == 0) return 0.0;
                    return static_cast<double>(jobController.getCurrentLine()) / static_cast<double>(jobController.getTotalLines());
                }
            }),

            std::make_unique<widgets::Label>([&jobController, this]() {
                return String(String(jobController.getCurrentLine()) + "/" + String(jobController.getTotalLines()));
            }),

            std::make_unique<widgets::Switch<bool>>(
                [&jobController]() { return jobController.isActive(); },
                false,  // eager evaluation
                std::make_unique<widgets::Switch<bool>::Branch>(
                    true,
                    std::make_unique<widgets::LinearLayout>(
                        widgets::LinearLayoutStyle{.axis = widgets::Axis::Horizontal, .spacingMode = widgets::SpacingMode::SpaceAround},

                        std::make_unique<widgets::Button>(
                            widgets::ButtonProps{
                                .onPress = [&jobController, &motionState]() {
                                    if (motionState.getState() == MotionStateType::PAUSED) {
                                        jobController.resume();
                                    } else {
                                        jobController.pause();
                                    }
                                }
                            },
                            std::make_unique<widgets::Label>([&jobController, &motionState]() {
                                return motionState.getState() == MotionStateType::PAUSED ? "Resume" : "Pause";
                            })
                        ),

                        std::make_unique<widgets::Button>(
                            widgets::ButtonProps{
                                .onPress = [&jobController]() { jobController.abort(); }
                            },
                            std::make_unique<widgets::Label>("Abort")
                        )
                    )
                ),
                std::make_unique<widgets::Switch<bool>::Branch>(
                    false,
                    std::make_unique<widgets::Button>(
                        widgets::ButtonProps{
                            .onPress = [this]() { router()->popScreen(); }
                        },
                        std::make_unique<widgets::Label>("Back to Files")
                    )
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