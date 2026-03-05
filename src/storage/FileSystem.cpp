#include "FileSystem.hpp"

#include <SPIFFS.h>

bool fsInit()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS mount failed");
        return false;
    }

    Serial.println("SPIFFS mounted");
    return true;
}

std::vector<String> fsListFiles()
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

bool fsExists(const String& path)
{
    return SPIFFS.exists(path);
}

bool fsDelete(const String& path)
{
    if (!SPIFFS.exists(path))
        return false;

    return SPIFFS.remove(path);
}

File fsOpenRead(const String& path)
{
    return SPIFFS.open(path, "r");
}

File fsOpenWrite(const String& path)
{
    return SPIFFS.open(path, "w");
}