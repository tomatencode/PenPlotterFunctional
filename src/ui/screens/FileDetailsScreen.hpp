#pragma once

#include <functional>
#include <map>

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionState.hpp"
#include "storage/FileManager.hpp"
#include "jobController/JobController.hpp"

// Include related screens to enable navigation
#include "../framework/router/Router.hpp"
#include "PlottingScreen.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../components/FileList.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/leaves/Switch.hpp"
#include "../framework/widgets/layouts/VerticalLayout.hpp"
#include "../framework/widgets/layouts/HorizontalLayout.hpp"
#include "../components/PressHoldButton.hpp"

#include "config/job_config.hpp"

namespace ui {
namespace screens {

namespace {
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
}

class FileDetailsScreen : public Screen
{
public:
    FileDetailsScreen(const String& filename,
                      JobController& jobController,
                      MotionState& motionState,
                      FileManager& fileManager,
                      std::function<bool()> wifiStatusProvider
    )
    : Screen(
        widgets::make_widget<widgets::Switch<bool>>(
            [filename, &fileManager]() { return fileManager.fileExists(PLOTTING_DIRECTORY + "/" + filename); },
            true, // lazy evaluation
            std::make_unique<widgets::Switch<bool>::Branch>(
                true, 
                widgets::make_widget<widgets::VerticalLayout>(
                    widgets::VerticalLayoutStyle{},

                    widgets::make_widget<components::HeaderLine>(
                        filename.substring(0, filename.length() - 6),
                        wifiStatusProvider,
                        [this]() {
                        if (router()) {
                            router()->popScreen();
                        }
                    }),

                    widgets::make_widget<widgets::Label>([filename, &fileManager]() {
                        size_t size = fileManager.getFileSize(PLOTTING_DIRECTORY + "/" + filename);
                        return "Size: " + formatFileSize(size);
                    }),

                    widgets::make_widget<widgets::Label>([]() {
                        size_t plotTimeSeconds = 120;
                        return "Plot Time: " + formatPlotTime(plotTimeSeconds);
                    }),

                    widgets::make_widget<widgets::HorizontalLayout>(
                        widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::SpaceAround},

                        widgets::make_widget<widgets::Button>(
                            "Plot",
                            widgets::ButtonStyle(),
                            [this, filename, &jobController, &motionState, wifiStatusProvider]() {
                                jobController.start(filename);
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
                            2000
                        )
                    )
                )
            ),

            widgets::make_widget<widgets::Switch<bool>::Branch>(
                false,
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

                    widgets::make_widget<widgets::Label>("File does not exist!")
                )
            )
        )
    , 1)
    {}

    void onEnter() override {
        reload();
    }
};

} // namespace screens
} // namespace ui