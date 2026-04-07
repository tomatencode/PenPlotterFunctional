#pragma once

#include "ui/framework/screen/Screen.hpp"

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

#include "config/directorys_config.hpp"
#include "ui/styles/ButtonStyles.hpp"
#include "ui/screens/ScreensContext.hpp"

namespace ui {
namespace screens {

class FilesScreen : public Screen, public FileObserver
{
public:
    FilesScreen(const ScreensContext& ctx);
    ~FilesScreen();

    void reload() override;

private:
    ScreensContext _ctx;
    widgets::ScrollableVerticalLayout* _fileListLayout;

    void onFileEvent(FileEvent event, const std::string& path) override;
};

} // namespace screens
} // namespace ui