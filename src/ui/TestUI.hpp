#ifndef TEST_UI_HPP
#define TEST_UI_HPP

#include "screen/Screen.hpp"
#include "widgetSystem/Container.hpp"
#include "widgets/LabelWidget.hpp"
#include "widgets/ButtonWidget.hpp"
#include "router/Router.hpp"
#include "text/textSources/StaticText.hpp"
#include "widgets/ProgressBarWidget.hpp"
#include "text/textSources/FunctionText.hpp"

// Global screen pointers to keep them alive
Screen* g_screen1 = nullptr;
Screen* g_screen2 = nullptr;
Screen* g_screen3 = nullptr;

// Global router pointer for callbacks
static Router* g_router = nullptr;

// Optional: simple button style
static ButtonStyle defaultButtonStyle;

// Progress value for the progress bar (0-100)
static uint8_t g_progressValue = 80;

// Static callback functions
static void goToScreen2()
{
    if (g_router) g_router->pushScreen(g_screen2);
}

static void goToScreen3()
{
    if (g_router) g_router->pushScreen(g_screen3);
}

static void goBack()
{
    if (g_router) g_router->popScreen();
}

static void onButton1Pressed()
{
    g_progressValue = (g_progressValue >= 90) ? 100 : g_progressValue + 10; // Increment progress, max at 100
}

static void onButton2Pressed()
{
    g_progressValue = (g_progressValue >= 10) ? (g_progressValue - 10) : 0; // Decrement progress, floor at 0
}

static void onButton3Pressed()
{
    Serial.println("Button 3 pressed");
}

static void onButton4Pressed()
{
    Serial.println("Button 4 pressed");
}

static uint8_t getProgressValue()
{
    return g_progressValue;
}

static const char* progressTextFunc()
{
    static char buffer[10];
    snprintf(buffer, sizeof(buffer), "%d%%", g_progressValue);
    return buffer;
}

void setupTestUI(Router& router)
{
    g_router = &router; // Set global router pointer for callbacks
    // ---------------- Home Screen ----------------
    static StaticText homeText("Home");
    LabelWidget* label1 = new LabelWidget({0, 0, 20, 1}, homeText, {HorizontalAlignment::Center, VerticalAlignment::Top});

    static StaticText submenu1BtnText("Submenu 1");
    LabelWidget* sub1Label = new LabelWidget({0, 0, 18, 1}, submenu1BtnText);
    ButtonWidget* button1 = new ButtonWidget(
                                                {0, 1, 20, 1}, sub1Label, defaultButtonStyle,
                                                nullptr,
                                                goToScreen2,
                                                {HorizontalAlignment::Center, VerticalAlignment::Top}
                                            );

    static StaticText submenu2BtnText("Submenu 2");
    LabelWidget* sub2Label = new LabelWidget({0, 0, 18, 1}, submenu2BtnText);
    ButtonWidget* button2 = new ButtonWidget(
                                                {0, 2, 20, 1}, sub2Label, defaultButtonStyle,
                                                nullptr,
                                                goToScreen3,
                                                {HorizontalAlignment::Center, VerticalAlignment::Top}
                                            );

    Widget* screen1Widgets[] = { label1, button1, button2 };
    g_screen1 = new Screen(screen1Widgets, 3);

    // ---------------- Submenu 1 ----------------
    static StaticText submenu1TitleText("Submenu 1");
    LabelWidget* label2 = new LabelWidget({0, 0, 13, 1}, submenu1TitleText);

    ProgressBarWidget* progressBar = new ProgressBarWidget({1, 2, 13, 1}, getProgressValue);
    
    static FunctionText progressText(progressTextFunc);
    LabelWidget* progressLabel = new LabelWidget({15, 2, 5, 1}, progressText, {HorizontalAlignment::Right, VerticalAlignment::Top});

    static StaticText btn1Text("+ prog");
    LabelWidget* btn1Label = new LabelWidget({0, 0, 8, 1}, btn1Text);
    ButtonWidget* button3 = new ButtonWidget({0, 3, 10, 1}, btn1Label, defaultButtonStyle,
                                             onButton1Pressed,
                                             nullptr,
                                             {HorizontalAlignment::Center, VerticalAlignment::Top});

    static StaticText btn2Text("- prog");
    LabelWidget* btn2Label = new LabelWidget({0, 0, 8, 1}, btn2Text);
    ButtonWidget* button4 = new ButtonWidget({10, 3, 10, 1}, btn2Label, defaultButtonStyle,
                                             onButton2Pressed,
                                             nullptr,
                                             {HorizontalAlignment::Center, VerticalAlignment::Top});

    // Back button
    static StaticText backText("Back");
    LabelWidget* backLabel1 = new LabelWidget({0, 0, 6, 1}, backText);
    ButtonWidget* back_button_1 = new ButtonWidget({14, 0, 6, 1}, backLabel1, defaultButtonStyle,
                                                   nullptr,
                                                   goBack);

    Widget* screen2Widgets[] = { label2, progressBar, progressLabel, button3, button4, back_button_1 };
    g_screen2 = new Screen(screen2Widgets, 6);

    // ---------------- Submenu 2 ----------------
    static StaticText submenu2TitleText("Submenu 2");
    LabelWidget* label3 = new LabelWidget({0, 0, 13, 1}, submenu2TitleText);

    static StaticText btn3Text("Button 3");
    LabelWidget* btn3Label = new LabelWidget({0, 0, 18, 1}, btn3Text);
    ButtonWidget* button5 = new ButtonWidget({0, 2, 20, 1}, btn3Label, defaultButtonStyle,
                                             onButton3Pressed);

    static StaticText btn4Text("Button 4");
    LabelWidget* btn4Label = new LabelWidget({0, 0, 18, 1}, btn4Text);
    ButtonWidget* button6 = new ButtonWidget({0, 3, 20, 1}, btn4Label, defaultButtonStyle,
                                             onButton4Pressed);

    LabelWidget* backLabel2 = new LabelWidget({0, 0, 6, 1}, backText);
    ButtonWidget* back_button_2 = new ButtonWidget({14, 0, 6, 1}, backLabel2, defaultButtonStyle,
                                                   nullptr,
                                                   goBack);

    Widget* screen3Widgets[] = { label3, button5, button6, back_button_2 };
    g_screen3 = new Screen(screen3Widgets, 4);

    router.setScreen(g_screen1);
}

#endif