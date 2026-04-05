#include "FilesScreen.hpp"

#include <vector>
#include <memory>

namespace ui {
namespace screens {

FilesScreen::FilesScreen(JobController& jobController,
                         MotionState& motionState,
                         FileManager& fileManager,
                         std::function<bool()> wifiStatusProvider)
    : _jobController(jobController),
      _motionState(motionState),
      _fileManager(fileManager),
      _wifiStatusProvider(wifiStatusProvider),
      _fileListLayout(nullptr)
{
    _fileManager.registerFileObserver(this);

    auto headerLine = std::make_unique<components::HeaderLine>(components::HeaderLineProps{
        .textProvider = "Files",
        .wifiStatusProvider = _wifiStatusProvider,
        .onBackPress = [this]() {
            if (router()) router()->popScreen();
        }
    });

    auto monitorCurrentConditional = 
    std::make_unique<widgets::Conditional>(
        widgets::ConditionalProps{.condition = [&jobController]() { return jobController.isActive(); }},
        std::make_unique<widgets::Button>(
            widgets::ButtonProps{
                .style = ui::styles::listButtonStyle,
                .onPress = [this]() {
                    if (router()) {
                        auto currentJobScreen = std::make_unique<PlottingScreen>(_jobController, _motionState, _wifiStatusProvider);
                        router()->pushScreen(std::move(currentJobScreen));
                    }
                }
            },
            std::make_unique<widgets::Label>("Monitor Current")
        )
    );

    auto conditionalStartNewLabel = 
    std::make_unique<widgets::Conditional>(
        widgets::ConditionalProps{.condition = [&jobController]() { return jobController.isActive(); }},
            std::make_unique<widgets::Label>("Start New:")
    );

    auto fileListLayout = std::make_unique<widgets::ScrollableVerticalLayout>(
        widgets::ScrollableVerticalLayoutStyle{},
        std::move(monitorCurrentConditional),
        std::move(conditionalStartNewLabel)
    );
    _fileListLayout = fileListLayout.get();



    auto rootLayout = std::make_unique<widgets::LinearLayout>(
        widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},
        std::move(headerLine),
        std::move(fileListLayout)
    );

    setRoot(std::move(rootLayout), 1);
    reload();
}

FilesScreen::~FilesScreen() {
    _fileManager.unregisterFileObserver(this);
}

void FilesScreen::reload() {
    if (!_fileListLayout) return;

    // Remove all children except the first two (Monitor Current button and Start New label)
    size_t count = _fileListLayout->getChildCount();
    if (count > 2) {
        for (size_t i = count - 1; i > 1; i--) {
            _fileListLayout->removeChild(i);
        }
    }

    auto files = _fileManager.listFiles(PLOTTING_DIRECTORY + "/");
    for (const auto& file : files) {
        auto button = std::make_unique<widgets::Button>(
            widgets::ButtonProps{
                .style = ui::styles::listButtonStyle,
                .onPress = [this, file]() {
                    auto fileDetailsScreen = std::make_unique<FileDetailsScreen>(
                        file, _jobController, _motionState, _fileManager, _wifiStatusProvider
                    );
                    if (router()) router()->pushScreen(std::move(fileDetailsScreen));
                }
            },
            std::make_unique<widgets::Label>(file.substr(0, file.length() - 6))
        );
        _fileListLayout->addChild(std::move(button));
    }
}

void FilesScreen::onUnPause() {
    Screen::reload();
}

void FilesScreen::onFileEvent(FileEvent event, const std::string& path) {
    if (event == FileEvent::ADDED || event == FileEvent::REMOVED) {
        reload();
    }
}

} // namespace screens
} // namespace ui
