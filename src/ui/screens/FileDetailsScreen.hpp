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
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/leaves/Switch.hpp"
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
        std::make_unique<widgets::Switch<bool>>(
            widgets::SwitchProps{
                .selector = [filename, &fileManager]() { return fileManager.fileExists(PLOTTING_DIRECTORY + "/" + filename); },
                .evaluationMode = widgets::SwitchEvaluationMode::Lazy
            },
            std::make_unique<widgets::Switch<bool>::Branch>(
                true, 
                std::make_unique<widgets::LinearLayout>(
                    widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

                    std::make_unique<components::HeaderLine>(
                        filename.substring(0, filename.length() - 6),
                        wifiStatusProvider,
                        [this]() {
                        if (router()) {
                            router()->popScreen();
                        }
                    }),

                    std::make_unique<widgets::Spacer>(1, 1),

                    std::make_unique<widgets::LinearLayout>(
                        widgets::LinearLayoutStyle{
                            .axis = widgets::Axis::Horizontal,
                            .spacingMode = widgets::SpacingMode::SpaceBetween,
                            .marginLeft = 1,
                            .marginRight = 1
                        },

                        std::make_unique<widgets::LinearLayout>(
                            widgets::LinearLayoutStyle{
                                .axis = widgets::Axis::Vertical,
                                .horizontalAlign = widgets::HorizontalAlignment::Center
                            },
                            
                            std::make_unique<widgets::Button>(
                                widgets::ButtonProps{
                                    .onPress = [filename, &jobController]() {
                                        jobController.start(filename);
                                    }
                                },
                                std::make_unique<widgets::Label>("Plot")
                            ),
                            std::make_unique<components::PressHoldButton>(
                                "Delete",
                                components::PressHoldButtonStyle(),
                                [this, filename, &fileManager]() {
                                    fileManager.deleteFile(PLOTTING_DIRECTORY + "/" + filename);
                                    if (router()) {
                                        router()->popScreen();
                                    }
                                },
                                2000, // 2 second hold time
                                500 // 500ms release animation
                            )
                        ),
                        
                        std::make_unique<widgets::LinearLayout>(
                            widgets::LinearLayoutStyle{
                                .axis = widgets::Axis::Vertical,
                                .horizontalAlign = widgets::HorizontalAlignment::Center,
                            },

                            std::make_unique<widgets::Label>([filename, &fileManager]() {
                                size_t size = fileManager.getFileSize(PLOTTING_DIRECTORY + "/" + filename);
                                return formatFileSize(size);
                            }),

                            std::make_unique<widgets::Label>([]() {
                                size_t plotTimeSeconds = 120;
                                return formatPlotTime(plotTimeSeconds);
                            })
                        )
                    )
                )
            ),

            std::make_unique<widgets::Switch<bool>::Branch>(
                false,
                std::make_unique<widgets::LinearLayout>(
                    widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},

                    std::make_unique<components::HeaderLine>(
                        filename.substring(0, filename.length() - 6),
                        wifiStatusProvider,
                        [this]() {
                            if (router()) {
                                router()->popScreen();
                            }
                        }
                    ),

                    std::make_unique<widgets::Label>("File does not exist!")
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