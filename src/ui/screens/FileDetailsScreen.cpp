#include "FileDetailsScreen.hpp"

// Include related screens to enable navigation
#include "../framework/router/Router.hpp"
#include "PlottingScreen.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../components/FileList.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/layouts/VerticalLayout.hpp"
#include "../framework/widgets/layouts/HorizontalLayout.hpp"
#include "../components/PressHoldButton.hpp"

#include "config/job_config.hpp"

namespace ui {
namespace screens {

String formatFileSize(size_t bytes) {
    if (bytes < 1024) return String(bytes) + " B";
    else if (bytes < 1024 * 1024) return String(bytes / 1024.0, 2) + " KB";
    else if (bytes < 1024 * 1024 * 1024) return String(bytes / (1024.0 * 1024), 2) + " MB";
    else return String(bytes / (1024.0 * 1024 * 1024), 2) + " GB";
}

String formatPlotTime(size_t seconds) {
    size_t mins = seconds / 60;
    size_t secs = seconds % 60;
    return String(mins) + "m " + String(secs) + "s";
}

FileDetailsScreen::FileDetailsScreen(const String& filename,
                                     JobController& jobController,
                                     MotionState& motionState,
                                     FileManager& fileManager,
                                     std::function<bool()> wifiStatusProvider
                                    )
    : Screen(
        !fileManager.fileExists(PLOTTING_DIRECTORY + "/" + filename) ? (
            widgets::make_widget<widgets::VerticalLayout>(
                widgets::VerticalLayoutStyle{},

                widgets::make_widget<components::HeaderLine>(
                    filename.substring(0, filename.length() - 6),
                    wifiStatusProvider,
                    [this]() {
                        if (router()) {
                            router()->popScreen();
                        }
                    }
                ),

                widgets::make_widget<widgets::Label>("This file does not exist!")
            )
            ) : (
            widgets::make_widget<widgets::VerticalLayout>(
                widgets::VerticalLayoutStyle{},

                widgets::make_widget<components::HeaderLine>(filename.substring(0, filename.length() - 6), wifiStatusProvider, [this]() {
                    if (router()) {
                        router()->popScreen();
                    }
                }),

                widgets::make_widget<widgets::Label>([filename, &fileManager]() {
                    size_t size = fileManager.getFileSize(PLOTTING_DIRECTORY + "/" + filename);
                    return "Size: " + formatFileSize(size);
                }),

                widgets::make_widget<widgets::Label>([]() {
                    // Placeholder: In a real implementation, you would calculate this based on file contents or metadata
                    size_t plotTimeSeconds = 120; // Example fixed time
                    return "Plot Time: " + formatPlotTime(plotTimeSeconds);
                }),

                widgets::make_widget<widgets::HorizontalLayout>(
                    widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::SpaceAround},

                    widgets::make_widget<widgets::Button>(
                        "Plot",
                        widgets::ButtonStyle(),
                        [this, filename, &jobController, &motionState, wifiStatusProvider]() {
                            jobController.start(filename);
                            // plotting screen will be pushed by the UiOrchestrator
                            // when it receives the job started event
                        }
                    ),
                    widgets::make_widget<components::PressHoldButton>(
                        "Delete",
                        components::PressHoldButtonStyle(),
                        [this, filename, &fileManager]() {
                            fileManager.deleteFile(PLOTTING_DIRECTORY + "/" + filename);
                            if (router()) {
                                router()->popScreen();
                            }
                        },
                        2000 // Hold time in milliseconds
                    )
                )
            )
        )
    , 1) // Start with the plot button focused
{
}

} // namespace screens
} // namespace ui