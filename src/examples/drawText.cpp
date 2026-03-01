#include "examples/drawText.hpp"
#include <cmath>

// Simple vector font - each letter defined as stroke endpoints
// Using relative coordinates (0-10 unit grid)

struct Stroke {
    float x1, y1;
    float x2, y2;
};

// Font definitions - each character as a series of strokes
// These are on a 10mm grid, easily scalable
const Stroke fontA[] = {
    {1, 0, 5, 10}, {5, 10, 9, 0}, {2, 3, 8, 3}
};
const Stroke fontB[] = {
    {1, 0, 1, 10}, {1, 0, 8, 0}, {8, 0, 8, 5}, {8, 5, 1, 5}, {8, 5, 8, 10}, {8, 10, 1, 10}
};
const Stroke fontC[] = {
    {8, 0, 1, 0}, {1, 0, 1, 10}, {1, 10, 8, 10}
};
const Stroke fontD[] = {
    {1, 0, 1, 10}, {1, 0, 7, 0}, {7, 0, 8, 2}, {8, 2, 8, 8}, {8, 8, 7, 10}, {7, 10, 1, 10}
};
const Stroke fontE[] = {
    {8, 0, 1, 0}, {1, 0, 1, 10}, {1, 10, 8, 10}, {1, 5, 7, 5}
};
const Stroke fontF[] = {
    {1, 0, 1, 10}, {1, 10, 8, 10}, {1, 5, 7, 5}
};
const Stroke fontG[] = {
    {8, 9, 1, 9}, {1, 9, 1, 0}, {1, 0, 8, 0}, {8, 0, 8, 5}, {8, 5, 6, 5}
};
const Stroke fontH[] = {
    {1, 0, 1, 10}, {8, 0, 8, 10}, {1, 5, 8, 5}
};
const Stroke fontI[] = {
    {2, 0, 8, 0}, {5, 0, 5, 10}, {2, 10, 8, 10}
};
const Stroke fontJ[] = {
    {4, 10, 4, 0}, {4, 0, 7, 0}, {7, 0, 7, 4}, {2, 10, 8, 10}
};
const Stroke fontK[] = {
    {1, 0, 1, 10}, {8, 0, 1, 5}, {1, 5, 8, 10}
};
const Stroke fontL[] = {
    {1, 10, 1, 0}, {1, 0, 8, 0}
};
const Stroke fontM[] = {
    {1, 0, 1, 10}, {1, 10, 5, 10}, {5, 10, 5, 0}, {5, 10, 9, 10}, {9, 10, 9, 0}
};
const Stroke fontN[] = {
    {1, 0, 1, 10}, {1, 10, 8, 1}, {8, 1, 8, 10}
};
const Stroke fontO[] = {
    {2, 0, 8, 0}, {8, 0, 9, 2}, {9, 2, 9, 8}, {9, 8, 8, 10}, {8, 10, 2, 10}, {2, 10, 1, 8}, {1, 8, 1, 2}, {1, 2, 2, 0}
};
const Stroke fontP[] = {
    {1, 0, 1, 10}, {1, 10, 8, 10}, {8, 10, 8, 5}, {8, 5, 1, 5}
};
const Stroke fontQ[] = {
    {2, 0, 8, 0}, {8, 0, 9, 2}, {9, 2, 9, 8}, {9, 8, 8, 10}, {8, 10, 2, 10}, {2, 10, 1, 8}, {1, 8, 1, 2}, {1, 2, 2, 0}, {7, 2, 9, 0}
};
const Stroke fontR[] = {
    {1, 0, 1, 10}, {1, 10, 8, 10}, {8, 10, 8, 5}, {8, 5, 1, 5}, {4, 5, 8, 0}
};
const Stroke fontS[] = {
    {9, 10, 1, 10}, {1, 10, 1, 6}, {1, 6, 9, 3}, {9, 3, 9, 0}, {9, 0, 1, 0}
};
const Stroke fontT[] = {
    {1, 10, 9, 10}, {5, 0, 5, 10}
};
const Stroke fontU[] = {
    {1, 10, 1, 2}, {1, 2, 2, 0}, {2, 0, 7, 0}, {7, 0, 8, 2}, {8, 2, 8, 10}
};
const Stroke fontV[] = {
    {1, 10, 4.5, 0}, {4.5, 0, 8, 10}
};
const Stroke fontW[] = {
    {1, 10, 2.5, 0}, {2.5, 0, 5, 6}, {5, 6, 7.5, 0}, {7.5, 0, 9, 10}
};
const Stroke fontX[] = {
    {1, 0, 9, 10}, {9, 0, 1, 10}
};
const Stroke fontY[] = {
    {1, 10, 5, 5}, {5, 5, 9, 10}, {5, 5, 5, 0}
};
const Stroke fontZ[] = {
    {8, 10, 1, 10}, {1, 10, 8, 0}, {8, 0, 1, 0}
};

// Digit definitions
const Stroke font0[] = {
    {2, 0, 8, 0}, {8, 0, 9, 2}, {9, 2, 9, 8}, {9, 8, 8, 10}, {8, 10, 2, 10}, {2, 10, 1, 8}, {1, 8, 1, 2}, {1, 2, 2, 0}, {1, 2, 9, 8}
};
const Stroke font1[] = {
    {3, 0, 7, 0}, {5, 0, 5, 10}, {5, 10, 1, 8}
};
const Stroke font2[] = {
    {9, 0, 1, 0}, {1, 0, 1, 4}, {1, 4, 8, 5}, {8, 5, 8, 9}, {8, 9, 1, 9}
};
const Stroke font3[] = {
    {2, 0, 9, 0}, {9, 0, 9, 5}, {9, 5, 2, 5}, {9, 5, 9, 9}, {9, 9, 2, 9}
};
const Stroke font4[] = {
    {4, 9, 1, 4}, {1, 4, 9, 4}, {5, 6, 5, 0}
};
const Stroke font5[] = {
    {2, 0, 9, 0}, {9, 0, 9, 5}, {9, 5, 2, 5}, {2, 5, 2, 10}, {2, 10, 9, 10}
};
const Stroke font6[] = {
    {1, 5, 9, 5}, {9, 5, 9, 1}, {9, 1, 1, 1}, {1, 1, 1, 10}, {1, 10, 9, 10}
};
const Stroke font7[] = {
    {2, 10, 9, 10}, {9, 10, 2, 0}, {2, 5, 8, 5}
};
const Stroke font8[] = {
    {2, 5, 2, 0}, {2, 0, 8, 0}, {8, 0, 8, 5}, {8, 5, 9, 5}, {9, 5, 9, 10}, {9, 10, 1, 10}, {1, 10, 1 , 5}, {1, 5, 8, 5}
};
const Stroke font9[] = {
    {9, 4, 1, 4}, {1, 4, 1, 9}, {1, 9, 9, 9}, {9, 9, 9, 0}, {9, 0, 1, 0},
};

// Space
const Stroke fontSpace[] = {};

// Punctuation
const Stroke fontExclamation[] = {
    {4.5, 10, 4.5, 3}, {4, 1, 5, 1}
};
const Stroke fontPeriod[] = {
    {4, 0, 5, 0}
};
const Stroke fontComma[] = {
    {6, 2, 4, 0}
};
const Stroke fontQuestion[] = {
    {5, 0, 5, 1}, {5, 4, 5, 5}, {5, 5, 9, 5}, {9, 5, 9, 10}, {9, 10, 1, 10}
};


// Map characters to their stroke definitions
struct CharacterDef {
    const Stroke* strokes;
    uint8_t stroke_count;
};

// Helper function to get character definition
CharacterDef getCharacter(char c) {
    if (c >= 'A' && c <= 'Z') {
        const static CharacterDef uppercase[] = {
            {fontA, sizeof(fontA) / sizeof(Stroke)},
            {fontB, sizeof(fontB) / sizeof(Stroke)},
            {fontC, sizeof(fontC) / sizeof(Stroke)},
            {fontD, sizeof(fontD) / sizeof(Stroke)},
            {fontE, sizeof(fontE) / sizeof(Stroke)},
            {fontF, sizeof(fontF) / sizeof(Stroke)},
            {fontG, sizeof(fontG) / sizeof(Stroke)},
            {fontH, sizeof(fontH) / sizeof(Stroke)},
            {fontI, sizeof(fontI) / sizeof(Stroke)},
            {fontJ, sizeof(fontJ) / sizeof(Stroke)},
            {fontK, sizeof(fontK) / sizeof(Stroke)},
            {fontL, sizeof(fontL) / sizeof(Stroke)},
            {fontM, sizeof(fontM) / sizeof(Stroke)},
            {fontN, sizeof(fontN) / sizeof(Stroke)},
            {fontO, sizeof(fontO) / sizeof(Stroke)},
            {fontP, sizeof(fontP) / sizeof(Stroke)},
            {fontQ, sizeof(fontQ) / sizeof(Stroke)},
            {fontR, sizeof(fontR) / sizeof(Stroke)},
            {fontS, sizeof(fontS) / sizeof(Stroke)},
            {fontT, sizeof(fontT) / sizeof(Stroke)},
            {fontU, sizeof(fontU) / sizeof(Stroke)},
            {fontV, sizeof(fontV) / sizeof(Stroke)},
            {fontW, sizeof(fontW) / sizeof(Stroke)},
            {fontX, sizeof(fontX) / sizeof(Stroke)},
            {fontY, sizeof(fontY) / sizeof(Stroke)},
            {fontZ, sizeof(fontZ) / sizeof(Stroke)}
        };
        return uppercase[c - 'A'];
    } else if (c >= '0' && c <= '9') {
        const static CharacterDef digits[] = {
            {font0, sizeof(font0) / sizeof(Stroke)},
            {font1, sizeof(font1) / sizeof(Stroke)},
            {font2, sizeof(font2) / sizeof(Stroke)},
            {font3, sizeof(font3) / sizeof(Stroke)},
            {font4, sizeof(font4) / sizeof(Stroke)},
            {font5, sizeof(font5) / sizeof(Stroke)},
            {font6, sizeof(font6) / sizeof(Stroke)},
            {font7, sizeof(font7) / sizeof(Stroke)},
            {font8, sizeof(font8) / sizeof(Stroke)},
            {font9, sizeof(font9) / sizeof(Stroke)}
        };
        return digits[c - '0'];
    } else if (c == ' ') {
        return {fontSpace, 0};
    } else if (c == '!') {
        return {fontExclamation, sizeof(fontExclamation) / sizeof(Stroke)};
    } else if (c == '.') {
        return {fontPeriod, sizeof(fontPeriod) / sizeof(Stroke)};
    } else if (c == ',') {
        return {fontComma, sizeof(fontComma) / sizeof(Stroke)};
    } else if (c == '?') {
        return {fontQuestion, sizeof(fontQuestion) / sizeof(Stroke)};
    }
    
    // Default: return empty
    return {fontSpace, 0};
}

void DrawText(
    const char* text,
    float x_mm,
    float y_mm,
    float size_mm,
    float draw_speed_mm_per_s,
    float move_speed_mm_per_s,
    MotionSystem& motionSystem,
    Servo& penServo,
    float pen_up_position,
    float pen_down_position
) {
    float char_width = 11.0f * size_mm / 10.0f;  // Each char is ~10 units wide, +1 for spacing
    float current_x = x_mm;
    float current_y = y_mm;

    float last_x = 0.0f;
    float last_y = 0.0f;
    bool last_valid = false;
    const float esp = 0.001f; // tolerance in mm for coordinate comparison

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        CharacterDef charDef = getCharacter(c);

        // Draw each stroke of the character
        for (uint8_t s = 0; s < charDef.stroke_count; s++) {
            const Stroke& stroke = charDef.strokes[s];

            // Scale and translate stroke endpoints
            float x1 = current_x + (stroke.x1 * size_mm / 10.0f);
            float y1 = current_y + (stroke.y1 * size_mm / 10.0f);
            float x2 = current_x + (stroke.x2 * size_mm / 10.0f);
            float y2 = current_y + (stroke.y2 * size_mm / 10.0f);

            // Decide whether we need to lift pen: only lift if the start
            // of this stroke is not equal to the end of the previously
            // drawn stroke (with a small tolerance).
            bool needLift = true;
            if (last_valid) {
                float dx = x1 - last_x;
                float dy = y1 - last_y;
                if (std::fabs(dx) <= esp && std::fabs(dy) <= esp) {
                    needLift = false;
                }
            }

            if (needLift) {
                penServo.write(pen_up_position);
                delay(100);

                // Move to start of stroke
                motionSystem.moveToXY({x1, y1}, move_speed_mm_per_s);

                // Lower pen
                penServo.write(pen_down_position);
                delay(100);
            } else {
                // Already at the correct start point - ensure pen is down
                penServo.write(pen_down_position);
                delay(20);
            }

            // Draw stroke
            motionSystem.moveToXY({x2, y2}, draw_speed_mm_per_s);

            // Update last drawn end point
            last_x = x2;
            last_y = y2;
            last_valid = true;
        }

        // Move to next character position
        current_x += char_width;
    }

    // Final pen up
    penServo.write(pen_up_position);
}
