#ifndef TEST_UI_HPP
#define TEST_UI_HPP

#include "screenSystem/Screen.hpp"
#include "widgetSystem/Container.hpp"
#include "widgets/LabelWidget.hpp"
#include "widgets/ButtonWidget.hpp"
#include "router/Router.hpp"

// Global screen pointers to keep them alive
Screen* g_screen1 = nullptr;
Screen* g_screen2 = nullptr;
Screen* g_screen3 = nullptr;

void setupTestUI(Router& router)
{
    // Submenu 1 widgets
    LabelWidget* label2 = new LabelWidget({0, 0, 10, 1}, "Submenu 1");
    ButtonWidget* back_button_1 = new ButtonWidget({10, 0, 20, 1}, "Back", [&router](){ router.popScreen(); });
    ButtonWidget* button3 = new ButtonWidget({0, 1, 20, 1}, "Button 1", [](){ Serial.println("Button 1 pressed"); });
    ButtonWidget* button4 = new ButtonWidget({0, 2, 20, 1}, "Button 2", [](){ Serial.println("Button 2 pressed"); });

    Widget* screen2Widgets[] = { label2, button3, button4 };
    g_screen2 = new Screen(screen2Widgets, 3);

    // Submenu 2 widgets
    LabelWidget* label3 = new LabelWidget({0, 0, 10, 1}, "Submenu 2");
    ButtonWidget* back_button_2 = new ButtonWidget({10, 0, 20, 1}, "Back", [&router](){ router.popScreen(); });
    ButtonWidget* button5 = new ButtonWidget({0, 1, 20, 1}, "Button 3", [](){ Serial.println("Button 3 pressed"); });
    ButtonWidget* button6 = new ButtonWidget({0, 2, 20, 1}, "Button 4", [](){ Serial.println("Button 4 pressed"); });

    Widget* screen3Widgets[] = { label3, back_button_2, button5, button6 };
    g_screen3 = new Screen(screen3Widgets, 4);

    // Screen 1 widgets
    LabelWidget* label1 = new LabelWidget({0, 0, 20, 1}, "Home");
    ButtonWidget* button1 = new ButtonWidget({0, 1, 20, 1}, "Submenu 1", [&router](){ router.pushScreen(g_screen2); });
    ButtonWidget* button2 = new ButtonWidget({0, 2, 20, 1}, "Submenu 2", [&router](){ router.pushScreen(g_screen3); });

    Widget* screen1Widgets[] = { label1, button1, button2 };
    g_screen1 = new Screen(screen1Widgets, 3);

    router.setScreen(g_screen1);
}

#endif