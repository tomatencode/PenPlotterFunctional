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

#include "../framework/text/textSources/FunctionText.hpp"

namespace ui {
namespace screens {

PlottingScreen::PlottingScreen(const String& filename, JobManager& jobManager)
    : Screen(
        widgets::make_layout<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{.horizontalAlign = widgets::HorizontalAlignment::Center},
            widgets::make_widget<components::HeaderLine>(filename.substring(0, filename.length() - 6).c_str(), false),

            widgets::make_widget<widgets::ProgressBar>(widgets::ProgressBarStyle{}, [&jobManager]() {
                return jobManager.currentProgress() * 100.0; // Convert to percentage
             }),

            widgets::make_layout<widgets::HorizontalLayout>(
                widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::Even},
                widgets::make_widget<widgets::Label>(std::move(std::make_unique<FunctionText>([&jobManager]() {
                    return String(jobManager.getCurrentLine()) + "/" + String(jobManager.getTotalLines());
                })))
            ),

            widgets::make_layout<widgets::HorizontalLayout>(
                    widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::SpaceAround},

                    widgets::make_widget<widgets::ConditionalWidget>(
                        [&jobManager]() { return (jobManager.getCurrentLine() != jobManager.getTotalLines()); },
                        widgets::make_widget<widgets::Button>(
                            widgets::make_widget<widgets::Label>(std::move(std::make_unique<FunctionText>([&jobManager]() {
                                return jobManager.isJobPaused() ? "Resume" : "Pause";
                            }))),
                            widgets::ButtonStyle(),
                            [&jobManager]() {
                                if (jobManager.isJobPaused()) {
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
                            widgets::make_widget<widgets::Label>("Abort"),
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
                            widgets::make_widget<widgets::Label>("Back to Files"),
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
    jobManager.start("/" + filename); // Start the job when the screen is created
}

} // namespace screens
} // namespace ui