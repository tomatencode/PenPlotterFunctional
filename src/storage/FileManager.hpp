#pragma once

#include <FS.h>
#include <vector>
#include <Arduino.h>

#include "FileObserver.hpp"

class FileManager
{
public:
    FileManager();

    // Initialize the file system
    bool init();

    // Register an observer to be notified of file events (e.g., file added, removed)
    void registerFileObserver(FileObserver* observer);
    void unregisterFileObserver(FileObserver* observer);

    // List files in the root directory
    std::vector<String> listFiles();

    // Check if a file exists
    bool fileExists(const String& path);

    // Delete a file
    bool deleteFile(const String& path);

    // Open a file for reading
    File openFileRead(const String& path);

    // Open a file for writing
    File openFileWrite(const String& path);

    // Get the size of a file
    size_t getFileSize(const String& path);

private:
    std::vector<FileObserver*> _observers;

    // Internal method to notify observers of file events
    void notifyFileEvent(FileEvent event, const String& path);
};