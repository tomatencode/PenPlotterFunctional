#include "FileDetailsScreen.hpp"

// Include related screens to enable navigation
#include "../framework/router/Router.hpp"
#include "PlottingScreen.hpp"

// Include components and widgets used in this screen
#include "../components/HeaderLine.hpp"
#include "../components/FileList.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/layouts/VerticalLayout.hpp"
#include "../framework/widgets/layouts/HorizontalLayout.hpp"
#include "../framework/text/textSources/FunctionText.hpp"

// include storage for file details
#include "storage/FileSystem.hpp"



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

std::unique_ptr<ui::TextSource> createFileSizeTextSource(const String& filename) {
    return std::make_unique<FunctionText>([filename]() {
        size_t size = storage::fsFileSize("/" + filename);
        return "Size: " + formatFileSize(size);
    });
}

std::unique_ptr<ui::TextSource> createPlotTimeTextSource(const String& filename) {
    return std::make_unique<FunctionText>([filename]() {
        // Placeholder: In a real implementation, you would calculate this based on file contents or metadata
        size_t plotTimeSeconds = 120; // Example fixed time
        return "Plot Time: " + formatPlotTime(plotTimeSeconds);
    });
}

FileDetailsScreen::FileDetailsScreen(const String& filename, JobManager& jobManager)
    : Screen(
        !storage::fsExists("/" + filename) ? (
            widgets::make_layout<widgets::VerticalLayout>(
                widgets::VerticalLayoutStyle{},

                widgets::make_widget<components::HeaderLine>(filename.substring(0, filename.length() - 6).c_str(), true, [this]() {
                    if (router()) {
                        router()->popScreen(); // Go back to the previous screen
                    }
                }),

                widgets::make_widget<widgets::LabelWidget>("This file does not exist!")
            )
            ) : (
            widgets::make_layout<widgets::VerticalLayout>(
                widgets::VerticalLayoutStyle{},

                widgets::make_widget<components::HeaderLine>(filename.substring(0, filename.length() - 6).c_str(), true, [this]() {
                    if (router()) {
                        router()->popScreen(); // Go back to the previous screen
                    }
                }),

                widgets::make_widget<widgets::LabelWidget>(createFileSizeTextSource(filename)),

                widgets::make_widget<widgets::LabelWidget>(createPlotTimeTextSource(filename)),

                widgets::make_layout<widgets::HorizontalLayout>(
                    widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::SpaceAround},

                    widgets::make_widget<widgets::ButtonWidget>(
                        widgets::make_widget<widgets::LabelWidget>("Plot"),
                        widgets::ButtonStyle(),
                        [filename, this, &jobManager]() {
                            PlottingScreen* plottingScreen = new PlottingScreen(filename, jobManager);
                            if (router()) {
                                router()->pushScreen(plottingScreen);
                            }
                        }
                    ),
                    widgets::make_widget<widgets::ButtonWidget>(
                        widgets::make_widget<widgets::LabelWidget>("Delete"),
                        widgets::ButtonStyle(),
                        [filename]() {
                            // Handle delete file action
                        }
                    )
                )
            )
        )
    , 1) // Start with the plot button focused
{
}

} // namespace screens
} // namespace ui