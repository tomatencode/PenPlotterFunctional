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


PlottingScreen::PlottingScreen(const String& filename, JobManager& jobManager, MotionStateManager& ms, bool alreadyStarted)
    : Screen(
        widgets::make_widget<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{.horizontalAlign = widgets::HorizontalAlignment::Center},
            widgets::make_widget<components::HeaderLine>(filename.substring(0, filename.length() - 6), false),

            widgets::make_widget<widgets::ProgressBar>(widgets::ProgressBarStyle{}, [&jobManager]() {
                return static_cast<double>(jobManager.getCurrentLine()) / static_cast<double>(jobManager.getTotalLines());
             }),

            widgets::make_widget<widgets::HorizontalLayout>(
                widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::Even},
                widgets::make_widget<widgets::Label>([&jobManager]() {
                    return String(jobManager.getCurrentLine()) + "/" + String(jobManager.getTotalLines());
                })
            ),

            widgets::make_widget<widgets::HorizontalLayout>(
                    widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::SpaceAround},

                    widgets::make_widget<widgets::ConditionalWidget>(
                        [&jobManager]() { return (jobManager.getCurrentLine() != jobManager.getTotalLines()); },
                        widgets::make_widget<widgets::Button>(
                            [&jobManager, &ms]() {
                                return ms.getState() == MotionState::PAUSED ? "Resume" : "Pause";
                            },
                            widgets::ButtonStyle(),
                            [&jobManager, &ms]() {
                                if (ms.getState() == MotionState::PAUSED) {
                                    jobManager.resume();
                                } else {
                                    jobManager.pause();
                                }
                            }
                        )
                    ),

                    widgets::make_widget<widgets::ConditionalWidget>(
                        [&jobManager]() { return jobManager.getCurrentLine() != jobManager.getTotalLines(); },
                        widgets::make_widget<widgets::Button>(
                            "Abort",
                            widgets::ButtonStyle(),
                            [this, &jobManager]() {
                                jobManager.abort();
                                router()->popScreen(); // Go back to the previous screen
                            }
                        )
                    ),

                    widgets::make_widget<widgets::ConditionalWidget>(
                        [&jobManager]() { return jobManager.getCurrentLine() == jobManager.getTotalLines(); },
                        widgets::make_widget<widgets::Button>(
                            "Back to Files",
                            widgets::ButtonStyle(),
                            [this, &jobManager]() {
                                jobManager.abort();
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
        jobManager.start("/" + filename);
    }
}

} // namespace screens
} // namespace ui