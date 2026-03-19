#include "TestUI.hpp"
#include "SecondaryScreen.hpp"
#include "../framework/router/Router.hpp"

#include "../framework/widgets/Builder.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"

using namespace ui;
using namespace widgets;

TestScreen::TestScreen()
    : Screen(
        make_layout<VerticalLayout>(LayoutStyle(),
            make_widget<LabelWidget>("Pen Plotter UI"),
            make_widget<ButtonWidget>(
                make_widget<LabelWidget>("Next Screen"),
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