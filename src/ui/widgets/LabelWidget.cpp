#include "LabelWidget.hpp"

LabelWidget::LabelWidget(Rect box, const char* text) : Widget(box), text(text) {}

void LabelWidget::render(Renderer& r, Rect canvasBox)
{
    // Calculate absolute position on the display
    int absX = canvasBox.x + box().x;
    int absY = canvasBox.y + box().y;

    // clip text if it exceeds widget width
    int maxChars = box().w; // assuming 1 char per column
    char clippedText[maxChars + 1];
    strncpy(clippedText, text, maxChars);
    clippedText[maxChars] = '\0'; // ensure null-termination

    r.drawTextToBuffer(absX, absY, clippedText);
}