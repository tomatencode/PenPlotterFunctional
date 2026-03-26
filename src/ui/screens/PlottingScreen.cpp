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

namespace ui {
namespace screens {


PlottingScreen::PlottingScreen(const String& filename,
                               JobController& jobController,
                               MotionState& motionState,
                               std::function<bool()> wifiStatusProvider
                              )
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{.horizontalAlign = widgets::HorizontalAlignment::Center},
            widgets::make_widget<components::HeaderLine>(filename.substring(0, filename.length() - 6), wifiStatusProvider),

            widgets::make_widget<widgets::ProgressBar>(widgets::ProgressBarStyle{}, [&jobController, this]() {
                if (_jobOutdated) return 1.0; // Show full progress if a new job has started
                if (jobController.getTotalLines() == 0) return 0.0; // Avoid division by zero
                return static_cast<double>(jobController.getCurrentLine()) / static_cast<double>(jobController.getTotalLines());
             }),

            widgets::make_widget<widgets::HorizontalLayout>(
                widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::Even},
                widgets::make_widget<widgets::Label>([&jobController, this]() {
                    if (_jobOutdated) return String("New job started");
                    return String(String(jobController.getCurrentLine()) + "/" + String(jobController.getTotalLines()));
                })
            ),

            widgets::make_widget<widgets::HorizontalLayout>(
                    widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::SpaceAround},

                    widgets::make_widget<widgets::ConditionalWidget>(
                        [this]() { return !_completed && !_jobOutdated; },
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
                        )
                    ),

                    widgets::make_widget<widgets::ConditionalWidget>(
                        [this]() { return !_completed; },
                        widgets::make_widget<widgets::Button>(
                            "Abort",
                            widgets::ButtonStyle(),
                            [this, &jobController]() {
                                jobController.abort();
                                router()->popScreen(); // Go back to the previous screen
                            }
                        )
                    ),

                    widgets::make_widget<widgets::ConditionalWidget>(
                        [this]() { return _completed; },
                        widgets::make_widget<widgets::Button>(
                            "Back to Files",
                            widgets::ButtonStyle(),
                            [this, &jobController]() {
                                router()->popScreen(); // Go back to the previous screen
                            }
                        )
                    )
                )
            )
        ), _filename(filename), _jobController(jobController), _completed(false), _jobOutdated(false)
{
    jobController.registerObserver(this);

    if (!jobController.isActive()) {
        jobController.start("/" + filename);
    }
}

void PlottingScreen::onJobEvent(const JobEventType& event) {
    if (event.type == JobEvent::COMPLETED && event.filename == "/" + _filename) {
        Serial.println("Job completed");
        _completed = true;
    }
    if (event.type == JobEvent::STARTED && event.filename != "/" + _filename) {
        Serial.println("New job started while on PlottingScreen");
        _completed = true;
        _jobOutdated = true;
        if (router() && router()->top() == this) {  
            router()->popScreen(); // Go back to the previous screen
        }
    }
    else if (event.type == JobEvent::ABORTED && event.filename == "/" + _filename) {
        Serial.println("Job aborted");
        _completed = true;
        if (router() && router()->top() == this) {  
            router()->popScreen(); // Go back to the previous screen
        }
    }
}

} // namespace screens
} // namespace ui