#ifndef TEST_UI_HPP
#define TEST_UI_HPP

#include "screenSystem/Screen.hpp"
#include "widgetSystem/Container.hpp"
#include "widgets/LabelWidget.hpp"
#include "widgets/ButtonWidget.hpp"
#include "router/Router.hpp"

// Global counters for demo
int button1Pressed = 0;
int button2Pressed = 0;

// Button callbacks
void onButton1Press() { button1Pressed++; }
void onButton2Press() { button2Pressed++; }

void setupTestUI(Router& router)
{
    // Screen 1 widgets - use dynamic allocation
    LabelWidget* label1 = new LabelWidget({0, 0, 20, 1}, "Screen 1");
    ButtonWidget* button1 = new ButtonWidget({0, 1, 20, 1}, "Button 1", onButton1Press);
    ButtonWidget* button2 = new ButtonWidget({0, 2, 20, 1}, "Button 2", onButton2Press);

    Widget* screen1Widgets[] = { label1, button1, button2 };
    Screen* screen1 = new Screen(screen1Widgets, 3);

    router.setScreen(screen1);
}

#endif