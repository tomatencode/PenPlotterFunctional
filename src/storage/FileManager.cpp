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

std::vector<String> FileManager::listFiles()
{
    std::vector<String> files;

    File root = SPIFFS.open("/");

    File file = root.openNextFile();

    while (file)
    {
        files.push_back(String(file.name()));
        file = root.openNextFile();
    }

    return files;
}

bool FileManager::fileExists(const String& path)
{
    return SPIFFS.exists(path);
}

bool FileManager::deleteFile(const String& path)
{
    if (!SPIFFS.exists(path))
        return false;

    bool success = SPIFFS.remove(path);

    if (success)
    {
        notifyFileEvent(FileEvent::REMOVED, path);
    }
    
    return success;
}

File FileManager::openFileRead(const String& path)
{
    return SPIFFS.open(path, "r");
}

File FileManager::openFileWrite(const String& path)
{
    bool existed = fileExists(path);
    
    File file = SPIFFS.open(path, "w");

    if (file)
    {
        if (!existed)
        {
            notifyFileEvent(FileEvent::ADDED, path);
        }
        else
        {
            notifyFileEvent(FileEvent::UPDATED, path);
        }
    }

    return file;
}

size_t FileManager::getFileSize(const String& path)
{
    File file = SPIFFS.open(path);
    if (file)
    {
        size_t size = file.size();
        file.close();
        return size;
    }
    return 0;
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

void FileManager::notifyFileEvent(FileEvent event, const String& path)
{
    for (auto observer : _observers) {
        observer->onFileEvent(event, path);
    }
}