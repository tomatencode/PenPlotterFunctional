#ifndef TEST_UI_HPP
#define TEST_UI_HPP

#include "screen/Screen.hpp"
#include "widgetSystem/Container.hpp"
#include "widgets/LabelWidget.hpp"
#include "widgets/ButtonWidget.hpp"
#include "router/Router.hpp"
#include "text/textSources/StaticText.hpp"

// Global screen pointers to keep them alive
Screen* g_screen1 = nullptr;
Screen* g_screen2 = nullptr;
Screen* g_screen3 = nullptr;

// Optional: simple button style
static ButtonStyle defaultButtonStyle;

void setupTestUI(Router& router)
{
    // ---------------- Home Screen ----------------
    static StaticText homeText("Home");
    LabelWidget* label1 = new LabelWidget({0, 0, 20, 1}, homeText, {HorizontalAlignment::Center, VerticalAlignment::Top});

    static StaticText submenu1BtnText("Submenu 1");
    LabelWidget* sub1Label = new LabelWidget({0, 0, 18, 1}, submenu1BtnText);
    ButtonWidget* button1 = new ButtonWidget(
                                                {0, 1, 20, 1}, sub1Label, defaultButtonStyle,
                                                nullptr,
                                                [&router](){ router.pushScreen(g_screen2); },
                                                {HorizontalAlignment::Center, VerticalAlignment::Top}
                                            );

    static StaticText submenu2BtnText("Submenu 2");
    LabelWidget* sub2Label = new LabelWidget({0, 0, 18, 1}, submenu2BtnText);
    ButtonWidget* button2 = new ButtonWidget(
                                                {0, 2, 20, 1}, sub2Label, defaultButtonStyle,
                                                nullptr,
                                                [&router](){ router.pushScreen(g_screen3); },
                                                {HorizontalAlignment::Center, VerticalAlignment::Top}
                                            );

    Widget* screen1Widgets[] = { label1, button1, button2 };
    g_screen1 = new Screen(screen1Widgets, 3);

    // ---------------- Submenu 1 ----------------
    static StaticText submenu1TitleText("Submenu 1");
    LabelWidget* label2 = new LabelWidget({0, 0, 13, 1}, submenu1TitleText);

    static StaticText btn1Text("Button 1");
    LabelWidget* btn1Label = new LabelWidget({0, 0, 18, 1}, btn1Text);
    ButtonWidget* button3 = new ButtonWidget({0, 2, 20, 1}, btn1Label, defaultButtonStyle,
                                             [](){ Serial.println("Button 1 pressed"); });

    static StaticText btn2Text("Button 2");
    LabelWidget* btn2Label = new LabelWidget({0, 0, 18, 1}, btn2Text);
    ButtonWidget* button4 = new ButtonWidget({0, 3, 20, 1}, btn2Label, defaultButtonStyle,
                                             [](){ Serial.println("Button 2 pressed"); });

    // Back button
    static StaticText backText("Back");
    LabelWidget* backLabel1 = new LabelWidget({0, 0, 6, 1}, backText);
    ButtonWidget* back_button_1 = new ButtonWidget({14, 0, 6, 1}, backLabel1, defaultButtonStyle,
                                                   nullptr,
                                                   [&router](){ router.popScreen(); });

    Widget* screen2Widgets[] = { label2, button3, button4, back_button_1 };
    g_screen2 = new Screen(screen2Widgets, 4);

    // ---------------- Submenu 2 ----------------
    static StaticText submenu2TitleText("Submenu 2");
    LabelWidget* label3 = new LabelWidget({0, 0, 13, 1}, submenu2TitleText);

    static StaticText btn3Text("Button 3");
    LabelWidget* btn3Label = new LabelWidget({0, 0, 18, 1}, btn3Text);
    ButtonWidget* button5 = new ButtonWidget({0, 2, 20, 1}, btn3Label, defaultButtonStyle,
                                             [](){ Serial.println("Button 3 pressed"); });

    static StaticText btn4Text("Button 4");
    LabelWidget* btn4Label = new LabelWidget({0, 0, 18, 1}, btn4Text);
    ButtonWidget* button6 = new ButtonWidget({0, 3, 20, 1}, btn4Label, defaultButtonStyle,
                                             [](){ Serial.println("Button 4 pressed"); });

    LabelWidget* backLabel2 = new LabelWidget({0, 0, 6, 1}, backText);
    ButtonWidget* back_button_2 = new ButtonWidget({14, 0, 6, 1}, backLabel2, defaultButtonStyle,
                                                   nullptr,
                                                   [&router](){ router.popScreen(); });

    Widget* screen3Widgets[] = { label3, button5, button6, back_button_2 };
    g_screen3 = new Screen(screen3Widgets, 4);

    router.setScreen(g_screen1);
}

#endif