#include "TestUI.hpp"
#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"

#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"

TestScreen::TestScreen()
    : Screen(
        ui::widgets::make_layout<VerticalLayout>(LayoutStyle(),
            ui::widgets::make_widget<LabelWidget>("Pen Plotter UI"),
            ui::widgets::make_widget<ButtonWidget>(
                ui::widgets::make_widget<LabelWidget>("Next Screen"),
                ButtonStyle(),
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

Screen* createTestScreen()
{
    static TestScreen instance;
    return &instance;
}
