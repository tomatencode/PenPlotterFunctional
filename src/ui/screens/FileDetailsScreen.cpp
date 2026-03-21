#include "FileDetailsScreen.hpp"

// Include related screens to enable navigation
#include "../framework/router/Router.hpp"

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

String shortFilename(const String& filename) {
    if (filename.endsWith(".gcode")) {
        return filename.substring(0, filename.length() - 6);
    }
    return filename;
}

FileDetailsScreen::FileDetailsScreen(const String& filename)
    : Screen(
        widgets::make_layout<widgets::VerticalLayout>(
            widgets::VerticalLayoutStyle{},
            widgets::make_widget<components::HeaderLine>(shortFilename(filename).c_str(), true, [this]() {
                if (router()) {
                    router()->popScreen(); // Go back to the previous screen
                }
            }),

            widgets::make_widget<widgets::LabelWidget>(std::unique_ptr<ui::TextSource>(new FunctionText([filename]() {
                return "Size: 0 B"; // Placeholder, replace with actual file size if available
            }))),

            widgets::make_widget<widgets::LabelWidget>(std::unique_ptr<ui::TextSource>(new FunctionText([filename]() {
                return ("Plot Time: " + formatPlotTime(100)).c_str(); // Placeholder, replace with actual plot time if available
            }))),

            widgets::make_layout<widgets::HorizontalLayout>(
                widgets::HorizontalLayoutStyle{.spacingMode = widgets::SpacingMode::SpaceAround},

                widgets::make_widget<widgets::ButtonWidget>(
                    widgets::make_widget<widgets::LabelWidget>("Plot"),
                    widgets::ButtonStyle(),
                    [filename]() {
                        // Handle plot file action
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
{
}

} // namespace screens
} // namespace ui