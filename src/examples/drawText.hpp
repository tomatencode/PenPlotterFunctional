#ifndef DRAW_TEXT_HPP
#define DRAW_TEXT_HPP

#include <Arduino.h>
#include "motion/CoreXYKinematics.hpp"
#include "motion/StepperAxis.hpp"
#include "motion/MotionSystem.hpp"
#include "motion/Pen.hpp"

/**
 * Draw text on the plotter at a specific location with custom size
 * 
 * @param text String to draw (supports A-Z, 0-9, and space)
 * @param x_mm X coordinate starting position in millimeters
 * @param y_mm Y coordinate starting position in millimeters
 * @param size_mm Font size in millimeters (base unit is 10mm per character)
 * @param speed_mm_per_s Drawing speed in millimeters per second
 * @param motionSystem Motion system reference
 * @param Pen
 */
void DrawText(
    const char* text,
    float x_mm,
    float y_mm,
    float size_mm,
    float move_speed_mm_per_s,
    float draw_speed_mm_per_s,
    MotionSystem& motionSystem,
    Pen& pen
);

#endif // DRAW_TEXT_HPP
