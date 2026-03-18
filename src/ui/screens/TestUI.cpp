#include "TestUI.hpp"
#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"

#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"

TestScreen::TestScreen()
    : ui::Screen(
        ui::widgets::make_layout<ui::widgets::VerticalLayout>(ui::widgets::LayoutStyle(),
            ui::widgets::make_widget<ui::widgets::LabelWidget>("Pen Plotter UI"),
            ui::widgets::make_widget<ui::widgets::ButtonWidget>(
                ui::widgets::make_widget<ui::widgets::LabelWidget>("Next Screen"),
                ui::widgets::ButtonStyle(),
                [this]() {
                    if (router()) {
                        static SecondaryScreen secondScreen;
                        router()->pushScreen(&secondScreen);
                    }
                }
            )
        )
    )
{
}

ui::Screen* createTestScreen()
{
    static TestScreen instance;
    return &instance;
}
