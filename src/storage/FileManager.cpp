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

std::vector<std::string> FileManager::listFiles(const std::string& directory)
{
    std::vector<std::string> files;

    File root = SPIFFS.open(directory.c_str());

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
    return SPIFFS.exists(path.c_str());
}

bool FileManager::deleteFile(const std::string& path)
{
    if (!SPIFFS.exists(path.c_str()))
        return false;

    bool success = SPIFFS.remove(path.c_str());

    if (success)
    {
        notifyFileEvent(FileEvent::REMOVED, path);
    }
    
    return success;
}

File FileManager::openFileRead(const std::string& path)
{
    return SPIFFS.open(path.c_str(), "r");
}

File FileManager::openFileWrite(const std::string& path)
{
    bool existed = fileExists(path);
    
    File file = SPIFFS.open(path.c_str(), "w");

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

size_t FileManager::getFileSize(const std::string& path)
{
    File file = SPIFFS.open(path.c_str());
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

void FileManager::notifyFileEvent(FileEvent event, const std::string& path)
{
    for (auto observer : _observers) {
        observer->onFileEvent(event, path);
    }
}