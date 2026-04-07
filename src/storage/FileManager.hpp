#pragma once

#include <FS.h>
#include <string>
#include <vector>

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

    // List files in a directory
    std::vector<std::string> listFiles(const std::string& directory);

    // Check if a file exists
    bool fileExists(const std::string& path);

    // Delete a file
    bool deleteFile(const std::string& path);

    // Rename or move a file
    bool renameFile(const std::string& oldPath, const std::string& newPath);

    // Open a file for reading
    File openFileRead(const std::string& path);

    // Open a file for writing
    File openFileWrite(const std::string& path);

    // Get the size of a file
    size_t getFileSize(const std::string& path);

private:
    std::vector<FileObserver*> _observers;

    // Internal method to notify observers of file events
    void notifyFileEvent(FileEvent event, const std::string& path);

    // Normalize a path to SPIFFS format: ensures leading '/' and no trailing '/'
    static std::string normalizePath(const std::string& path);
};