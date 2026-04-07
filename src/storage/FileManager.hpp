#pragma once

#include <FS.h>
#include <string>
#include <vector>

#include "FileObserver.hpp"

class FileManager
{
public:
    FileManager();

    bool init();

    void registerFileObserver(FileObserver* observer);
    void unregisterFileObserver(FileObserver* observer);

    std::vector<std::string> listFiles(const std::string& directory);
    bool fileExists(const std::string& path);
    size_t getFileSize(const std::string& path);
    bool deleteFile(const std::string& path);
    bool renameFile(const std::string& oldPath, const std::string& newPath);
    File openFileRead(const std::string& path);
    File openFileWrite(const std::string& path);

private:

    static std::string normalizePath(const std::string& path);

    void notifyFileEvent(FileEvent event, const std::string& path);

    std::vector<FileObserver*> _observers;
};