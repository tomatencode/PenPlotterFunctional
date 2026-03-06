#pragma once

#include "parser/GCodeParser.hpp"

// Expose the parser so tasks can use it
extern GCodeParser gcodeParser;

void initMachine();
void machineUpdate();