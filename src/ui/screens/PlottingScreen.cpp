#include "PlottingScreen.hpp"

// Include related screens to enable navigation
#include "FilesScreen.hpp"

// Include related screens to enable navigation
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


PlottingScreen::PlottingScreen(JobController& jobController,
                               MotionState& motionState,
                               std::function<bool()> wifiStatusProvider
                              )
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{.horizontalAlign = widgets::HorizontalAlignment::Center},
            widgets::make_widget<components::HeaderLine>(
                "Plotting",
                wifiStatusProvider
            ),

            widgets::make_widget<widgets::ProgressBar>(widgets::ProgressBarStyle{}, [&jobController, this]() {
                if (jobController.getTotalLines() == 0) return 0.0; // Avoid division by zero
                return static_cast<double>(jobController.getCurrentLine()) / static_cast<double>(jobController.getTotalLines());
             }),

            widgets::make_widget<widgets::HorizontalLayout>(
                widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::Even},
                widgets::make_widget<widgets::Label>([&jobController, this]() {
                    return String(String(jobController.getCurrentLine()) + "/" + String(jobController.getTotalLines()));
                })
            ),

            widgets::make_widget<widgets::ConditionalWidget>(
                [jobController]() { return jobController.isActive(); },
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
                        [this, &jobController]() {
                            jobController.abort();
                            // After aborting, the screen will automatically pop back
                            // to the previous screen via the observer callback
                        }
                    )
                )
            ),

            widgets::make_widget<widgets::ConditionalWidget>(
                [jobController]() { return !jobController.isActive(); },
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

PlottingScreen::~PlottingScreen() {
    _jobController.unregisterObserver(this);
}

void PlottingScreen::onJobEvent(const JobEventType& event) {
    if (event.type == JobEvent::ABORTED) {
        if (router() && router()->top() == this) {  
            router()->popScreen(); // Go back to the previous screen
        }
    }
}

} // namespace screens
} // namespace ui