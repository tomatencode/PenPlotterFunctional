#pragma once

#include <Arduino.h>

enum class FileEvent {
    ADDED = 0,
    REMOVED = 1
};

class FileObserver
{
public:
    virtual ~FileObserver() = default;

    // Method to be called when a file event occurs (e.g., file added, removed)
    virtual void onFileEvent(FileEvent event, const String& path) = 0;
};
