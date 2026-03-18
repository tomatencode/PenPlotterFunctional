#include "TestUI.hpp"
#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"

#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"

#include "../framework/widgets/layouts/VerticalLayout.hpp"

TestScreen::TestScreen()
    : Screen() // base initialized; we'll set root after member init
    , titleLabel(nullptr)
    , btnLabel(nullptr)
    , button(nullptr)
{
    // Create widgets on the heap and transfer ownership to the layout
    titleLabel = new LabelWidget("Pen Plotter UI");
    btnLabel = new LabelWidget("Next Screen");

    button = new ButtonWidget(std::unique_ptr<Widget>(btnLabel), ButtonStyle(), [this]() {
        if (router()) {
            static SecondaryScreen secondScreen;
            router()->pushScreen(&secondScreen);
        }
    });

    std::vector<std::unique_ptr<Widget>> children;
    children.emplace_back(std::unique_ptr<Widget>(titleLabel));
    children.emplace_back(std::unique_ptr<Widget>(button));

    initRoot(std::unique_ptr<Widget>(new VerticalLayout(std::move(children))));
}

Screen* createTestScreen()
{
    static TestScreen instance;
    return &instance;
}
