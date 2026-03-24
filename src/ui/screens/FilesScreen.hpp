#pragma once

#include "../framework/screen/Screen.hpp"

#include "systemServices/MotionStateManager.hpp"
#include "jobManager/JobManager.hpp"

#include "storage/FileObserver.hpp"

namespace ui {
namespace screens {

// The main screen of the UI
class FilesScreen : public ui::Screen, public FileObserver
{
public:
    FilesScreen(JobManager& jobManager, MotionStateManager& ms, FileManager& fileManager);
    ~FilesScreen();

    void onFileEvent(FileEvent event, const String& path) override;
private:
    FileManager& _fileManager;
};

} // namespace screens
} // namespace ui