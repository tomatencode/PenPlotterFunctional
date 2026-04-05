#pragma once

#include "ui/framework/screen/Screen.hpp"

#include "rtos/MotionState.hpp"
#include "jobController/JobController.hpp"

#include "storage/FileObserver.hpp"
#include "storage/FileManager.hpp"

// Include related screens to enable navigation
#include "ui/framework/router/Router.hpp"
#include "FileDetailsScreen.hpp"

// Include components and widgets used in this screen
#include "ui/components/HeaderLine.hpp"
#include "ui/framework/widgets/leaves/Button.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/framework/widgets/leaves/Conditional.hpp"
#include "ui/framework/widgets/layouts/LinearLayout.hpp"
#include "ui/framework/widgets/layouts/ScrollableVerticalLayout.hpp"

#include "config/job_config.hpp"
#include "ui/styles/ButtonStyles.hpp"

namespace ui {
namespace screens {

class FilesScreen : public Screen, public FileObserver
{
public:
    FilesScreen(JobController& jobController,
                MotionState& motionState,
                FileManager& fileManager,
                std::function<bool()> wifiStatusProvider
            );
    ~FilesScreen();

    void reload() override;

private:
    JobController& _jobController;
    MotionState& _motionState;
    FileManager& _fileManager;
    std::function<bool()> _wifiStatusProvider;
    widgets::ScrollableVerticalLayout* _fileListLayout;

    void onFileEvent(FileEvent event, const std::string& path) override;
};

} // namespace screens
} // namespace ui