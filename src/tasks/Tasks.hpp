#pragma once

#include <Arduino.h>

// Start all firmware tasks
void startSystemTasks();

// Task functions
void motionTask(void *parameter);
void systemTask(void *parameter);