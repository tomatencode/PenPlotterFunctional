#pragma once

#include <functional>
#include <map>

#include "ui/framework/screen/Screen.hpp"

#include "rtos/MotionState.hpp"
#include "jobController/JobController.hpp"
#include "jobController/JobObserver.hpp"

// Include related screens to enable navigation
#include "FilesScreen.hpp"
#include "ui/framework/router/Router.hpp"

// Include components and widgets used in this screen
#include "ui/components/HeaderLine.hpp"
#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/framework/widgets/leaves/ProgressBar.hpp"
#include "ui/framework/widgets/leaves/Conditional.hpp"

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
            std::make_unique<components::HeaderLine>(components::HeaderLineProps{
                .textProvider = [&jobController]() {
                    std::string filename = jobController.getCurrentFile();
                    if (filename.ends_with(".gcode")) {
                        filename = filename.substr(0, filename.length() - 6);
                    }
                    if (filename == "") {
                        filename = "No Job";
                    }
                    return filename;
                },
                .wifiStatusProvider = wifiStatusProvider,
                .onBackPress = [this]() {
                    if (router()) router()->popScreen();
                }
            }),

            std::make_unique<widgets::ProgressBar>(widgets::ProgressBarProps{
                .getProgress = [&jobController, this]() {
                    if (jobController.getTotalLines() == 0) return 0.0;
                    return static_cast<double>(jobController.getCurrentLine()) / static_cast<double>(jobController.getTotalLines());
                }
            }),

            std::make_unique<widgets::Label>([&jobController, this]() {
                return std::to_string(jobController.getCurrentLine()) + "/" + std::to_string(jobController.getTotalLines());
            }),

            std::make_unique<widgets::Conditional>(
                widgets::ConditionalProps{
                    .condition = [&jobController]() {
                        return jobController.isActive();
                    }
                },
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
            )
        )
    , 1)/*start with pause button focused*/, _jobController(jobController)
    {
        jobController.registerObserver(this);
    }

    ~PlottingScreen() {
        _jobController.unregisterObserver(this);
    }

    void onJobEvent(const JobEventType& event) override {
        if (event.type == JobEvent::ABORTED) {
            if (router()) {  
                router()->popScreen();
            }
        }
    }

private:
    JobController& _jobController;
};

} // namespace screens
} // namespace ui