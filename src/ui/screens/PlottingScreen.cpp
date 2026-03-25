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


PlottingScreen::PlottingScreen(const String& filename, JobController& jobController, MotionState& ms, bool alreadyStarted)
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{.horizontalAlign = widgets::HorizontalAlignment::Center},
            widgets::make_widget<components::HeaderLine>(filename.substring(0, filename.length() - 6), false),

            widgets::make_widget<widgets::ProgressBar>(widgets::ProgressBarStyle{}, [&jobController]() {
                return static_cast<double>(jobController.getCurrentLine()) / static_cast<double>(jobController.getTotalLines());
             }),

            widgets::make_widget<widgets::HorizontalLayout>(
                widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::Even},
                widgets::make_widget<widgets::Label>([&jobController]() {
                    return String(jobController.getCurrentLine()) + "/" + String(jobController.getTotalLines());
                })
            ),

            widgets::make_widget<widgets::HorizontalLayout>(
                    widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::SpaceAround},

                    widgets::make_widget<widgets::ConditionalWidget>(
                        [&jobController]() { return (jobController.getCurrentLine() != jobController.getTotalLines()); },
                        widgets::make_widget<widgets::Button>(
                            [&jobController, &ms]() {
                                return ms.getState() == MotionStateType::PAUSED ? "Resume" : "Pause";
                            },
                            widgets::ButtonStyle(),
                            [&jobController, &ms]() {
                                if (ms.getState() == MotionStateType::PAUSED) {
                                    jobController.resume();
                                } else {
                                    jobController.pause();
                                }
                            }
                        )
                    ),

                    widgets::make_widget<widgets::ConditionalWidget>(
                        [&jobController]() { return jobController.getCurrentLine() != jobController.getTotalLines(); },
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
                        [&jobController]() { return jobController.getCurrentLine() == jobController.getTotalLines(); },
                        widgets::make_widget<widgets::Button>(
                            "Back to Files",
                            widgets::ButtonStyle(),
                            [this, &jobController]() {
                                jobController.abort();
                                router()->popScreen(); // Go back to the previous screen
                            }
                        )
                    )
                )
            )
        )
    
{
    // Only start the job if it hasn't been started already (e.g., by WebInterface)
    if (!alreadyStarted) {
        jobController.start("/" + filename);
    }
}

} // namespace screens
} // namespace ui