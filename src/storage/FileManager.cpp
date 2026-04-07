#include "FileManager.hpp"

#include <SPIFFS.h>

FileManager::FileManager() {}


bool FileManager::init()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS mount failed");
        return false;
    }

    Serial.println("SPIFFS mounted");
    return true;
}

std::string FileManager::normalizePath(const std::string& path)
{
    std::string result = path;

    // Remove trailing slash
    if (!result.empty() && result.back() == '/')
        result.pop_back();

    // Ensure leading slash
    if (result.empty() || result.front() != '/')
        result = '/' + result;

    return result;
}

std::vector<std::string> FileManager::listFiles(const std::string& directory)
{
    std::vector<std::string> files;

    File root = SPIFFS.open(normalizePath(directory).c_str());

    File file = root.openNextFile();

    while (file)
    {
        files.push_back(std::string(file.name()));
        file = root.openNextFile();
    }

    return files;
}

bool FileManager::fileExists(const std::string& path)
{
    return SPIFFS.exists(normalizePath(path).c_str());
}

size_t FileManager::getFileSize(const std::string& path)
{
    File file = SPIFFS.open(normalizePath(path).c_str());
    if (file)
    {
        size_t size = file.size();
        file.close();
        return size;
    }
    return 0;
}

bool FileManager::deleteFile(const std::string& path)
{
    const std::string normalized = normalizePath(path);

    if (!SPIFFS.exists(normalized.c_str()))
        return false;

    bool success = SPIFFS.remove(normalized.c_str());

    if (success)
    {
        notifyFileEvent(FileEvent::REMOVED, normalized);
    }
    
    return success;
}

bool FileManager::renameFile(const std::string& oldPath, const std::string& newPath)
{
    const std::string normalizedOld = normalizePath(oldPath);
    const std::string normalizedNew = normalizePath(newPath);

    if (!SPIFFS.exists(normalizedOld.c_str()))
        return false;

    bool success = SPIFFS.rename(normalizedOld.c_str(), normalizedNew.c_str());

    if (success)
    {
        notifyFileEvent(FileEvent::REMOVED, normalizedOld);
        notifyFileEvent(FileEvent::ADDED, normalizedNew);
    }
    
    return success;
}

File FileManager::openFileRead(const std::string& path)
{
    return SPIFFS.open(normalizePath(path).c_str(), "r");
}

File FileManager::openFileWrite(const std::string& path)
{
    const std::string normalized = normalizePath(path);
    bool existed = fileExists(normalized);
    
    File file = SPIFFS.open(normalized.c_str(), "w");

    if (file)
    {
        if (!existed)
        {
            notifyFileEvent(FileEvent::ADDED, normalized);
        }
        else
        {
            notifyFileEvent(FileEvent::UPDATED, normalized);
        }
    }

    return file;
}

void FileManager::registerFileObserver(FileObserver* observer)
{
    if (observer == nullptr) return;
    
    // Avoid duplicate registrations
    for (auto obs : _observers) {
        if (obs == observer) return;
    }
    
    _observers.push_back(observer);
}

void FileManager::unregisterFileObserver(FileObserver* observer)
{
    if (observer == nullptr) return;
    
    for (size_t i = 0; i < _observers.size(); i++) {
        if (_observers[i] == observer) {
            _observers.erase(_observers.begin() + i);
            return;
        }
    }
}

void FileManager::notifyFileEvent(FileEvent event, const std::string& path)
{
    for (auto observer : _observers) {
        observer->onFileEvent(event, path);
    }
}