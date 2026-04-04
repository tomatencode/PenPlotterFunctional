#pragma once

#include <Arduino.h>

// Start all firmware tasks
void startRtosTasks();

// Task functions
void plottingTask(void *parameter);
void systemTask(void *parameter);