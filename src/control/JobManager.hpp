#pragma once

#include <Arduino.h>

void jobManagerInit();

void jobStart(String filename);
void jobStop();

void jobManagerUpdate();
bool jobRunning();