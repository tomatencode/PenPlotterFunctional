#pragma once

#include "screen/Screen.hpp"
#include "widgets/layouts/AbsoluteLayout.hpp"
#include "widgets/leaves/LabelWidget.hpp"
#include "widgets/leaves/ButtonWidget.hpp"
#include "router/Router.hpp"
#include "text/textSources/StaticText.hpp"
#include "widgets/leaves/ProgressBarWidget.hpp"
#include "text/textSources/FunctionText.hpp"
#include "widgets/layouts/ScrollableVerticalLayout.hpp"
#include "text/Glyph.hpp"

// Global screen pointers to keep them alive
Screen* g_screen1 = nullptr;
Screen* g_screen2 = nullptr;
Screen* g_screen3 = nullptr;

// Global router pointer for callbacks
static Router* g_router = nullptr;

// simple button styles
static ButtonStyle defaultButtonStyle;
static ButtonStyle MenueButtonStyle = {
    Glyph(GLYPH_NONE.code),
    Glyph(GLYPH_NONE.code),
    Glyph('>'),
    Glyph(GLYPH_NONE.code),
    Glyph('-'),
    Glyph(GLYPH_NONE.code)
};

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

static void onProgPlusPressed()
{
    g_progressValue = (g_progressValue >= 90) ? 100 : g_progressValue + 10; // Increment progress, max at 100
}

static void onProgMinusPressed()
{
    g_progressValue = (g_progressValue >= 10) ? (g_progressValue - 10) : 0; // Decrement progress, floor at 0
}

static void onButton1Pressed()
{
    Serial.println("Button 1 pressed");
}

static void onButton2Pressed()
{
    Serial.println("Button 2 pressed");
}

static void onButton3Pressed()
{
    Serial.println("Button 3 pressed");
}

static void onButton4Pressed()
{
    Serial.println("Button 4 pressed");
}

static void onButton5Pressed()
{
    Serial.println("Button 5 pressed");
}

static void onButton6Pressed()
{
    Serial.println("Button 6 pressed");
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
    LabelWidget* label1 = new LabelWidget(homeText);

    static StaticText submenu1BtnText("Submenu 1");
    LabelWidget* sub1Label = new LabelWidget(submenu1BtnText);
    ButtonWidget* sub1Button = new ButtonWidget(sub1Label, defaultButtonStyle,
                                                nullptr,
                                                goToScreen2);

    static StaticText submenu2BtnText("Submenu 2");
    LabelWidget* sub2Label = new LabelWidget(submenu2BtnText);
    ButtonWidget* sub2Button = new ButtonWidget(sub2Label, defaultButtonStyle,
                                                nullptr,
                                                goToScreen3);

    Widget* screen1Widgets[] = { label1, sub1Button, sub2Button };
    g_screen1 = new Screen(screen1Widgets, 3);

    // ---------------- Submenu 1 ----------------
    static StaticText submenu1TitleText("Submenu 1");
    LabelWidget* label2 = new LabelWidget(submenu1TitleText);

    ProgressBarWidget* progressBar = new ProgressBarWidget(13, 1, getProgressValue);
    
    static FunctionText progressText(progressTextFunc);
    LabelWidget* progressLabel = new LabelWidget(progressText);

    static StaticText ProgPlusText("+ prog");
    LabelWidget* ProgPlusLabel = new LabelWidget(ProgPlusText);
    ButtonWidget* ProgPlusButton = new ButtonWidget(ProgPlusLabel, defaultButtonStyle,
                                             onProgPlusPressed,
                                             nullptr);

    static StaticText ProgMinusText("- prog");
    LabelWidget* ProgMinusLabel = new LabelWidget(ProgMinusText);
    ButtonWidget* ProgMinusButton = new ButtonWidget(ProgMinusLabel, defaultButtonStyle,
                                             onProgMinusPressed,
                                             nullptr);

    // Back button
    static StaticText backText("Back");
    LabelWidget* backLabel1 = new LabelWidget(backText);
    ButtonWidget* back_button_1 = new ButtonWidget(backLabel1, defaultButtonStyle,
                                                   nullptr,
                                                   goBack);

    Widget* screen2Widgets[] = { label2, progressBar, progressLabel, ProgPlusButton, ProgMinusButton, back_button_1 };
    g_screen2 = new Screen(screen2Widgets, 6);

    // ---------------- Submenu 2 ----------------
    static StaticText submenu2TitleText("Submenu 2");
    LabelWidget* label3 = new LabelWidget(submenu2TitleText);

    static StaticText btn1Text("Button 1");
    LabelWidget* btn1Label = new LabelWidget(btn1Text);
    ButtonWidget* button1 = new ButtonWidget(btn1Label, MenueButtonStyle,
                                             onButton1Pressed);

    static StaticText btn2Text("Button 2");
    LabelWidget* btn2Label = new LabelWidget(btn2Text);
    ButtonWidget* button2 = new ButtonWidget(btn2Label, MenueButtonStyle,
                                             onButton2Pressed);

    static StaticText btn3Text("Button 3");
    LabelWidget* btn3Label = new LabelWidget(btn3Text);
    ButtonWidget* button3 = new ButtonWidget(btn3Label, MenueButtonStyle,
                                             onButton3Pressed);

    static StaticText btn4Text("Button 4");
    LabelWidget* btn4Label = new LabelWidget(btn4Text);
    ButtonWidget* button4 = new ButtonWidget(btn4Label, MenueButtonStyle,
                                             onButton4Pressed);

    static StaticText btn5Text("Button 5");
    LabelWidget* btn5Label = new LabelWidget(btn5Text);
    ButtonWidget* button5 = new ButtonWidget(btn5Label, MenueButtonStyle,
                                             onButton5Pressed);

    static StaticText btn6Text("Button 6");
    LabelWidget* btn6Label = new LabelWidget(btn6Text);
    ButtonWidget* button6 = new ButtonWidget(btn6Label, MenueButtonStyle,
                                             onButton6Pressed);


    Widget* scrollableChildren[] = { button1, button2, button3, button4, button5, button6 };
    ScrollableVerticalLayout* scrollableVerticalLayout = new ScrollableVerticalLayout(scrollableChildren, 6);

    LabelWidget* backLabel2 = new LabelWidget(backText);
    ButtonWidget* back_button_2 = new ButtonWidget(backLabel2, defaultButtonStyle,
                                                   nullptr,
                                                   goBack);

    Widget* screen3Widgets[] = { label3, scrollableVerticalLayout, back_button_2 };
    g_screen3 = new Screen(screen3Widgets, 3);

    router.setScreen(g_screen1);
}