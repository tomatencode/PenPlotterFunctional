#pragma once
#include <string>

enum class FileEvent {
    ADDED = 0,
    UPDATED = 1,
    REMOVED = 2
};

// not ThreadSafe - only to be used on same core as FileManager
class FileObserver
{
public:
    virtual ~FileObserver() = default;

    // Method to be called when a file event occurs (e.g., file added, removed)
    virtual void onFileEvent(FileEvent event, const std::string& path) = 0;
};
