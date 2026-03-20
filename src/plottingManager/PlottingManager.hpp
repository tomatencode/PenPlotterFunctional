#pragma once

#include "gcode/GCodeParser.hpp"

// Expose the parser so tasks can use it
extern GCodeParser gcodeParser;

void plottingManagerInit();
void plottingManagerUpdate();