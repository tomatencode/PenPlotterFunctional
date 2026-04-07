#include "WebInterface.hpp"
#include "config/directorys_config.hpp"

#include <string>
#include <functional>
#include <cstdlib>

void WebInterface::handleFileList()
{
    auto files = _fileManager.listFiles(PLOTTING_DIRECTORY);

    std::string json = "[";
    for (size_t i = 0; i < files.size(); i++)
    {
        if (i != 0) json += ",";
        json += "\"" + files[i] + "\"";
    }
    json += "]";

    _server.send(200, "application/json", json.c_str());
}

void WebInterface::handlePauseJob()
{
    _jobController.pause();
    _server.send(200, "text/plain", "Job paused");
}

void WebInterface::handleResumeJob()
{
    _jobController.resume();
    _server.send(200, "text/plain", "Job resumed");
}

void WebInterface::handleStartJob()
{
    if (!_server.hasArg("file"))
    {
        _server.send(400, "text/plain", "Error: Missing 'file' parameter");
        return;
    }

    String filename = _server.arg("file");
    std::string filepath = PLOTTING_DIRECTORY + filename.c_str();

    if (!_fileManager.fileExists(filepath))
    {
        _server.send(404, "text/plain", "Error: File not found");
        return;
    }

    _jobController.start(filename.c_str());
    _server.send(200, "text/plain", "Job started");
}

void WebInterface::handleAbortJob()
{
    _jobController.abort();
    _server.send(200, "text/plain", "Job aborted");
}

void WebInterface::handleUpload()
{
    HTTPUpload& upload = _server.upload();

    if (upload.status == UPLOAD_FILE_START)
    {
        // Validate filename
        if (!validateFileName(upload.filename.c_str()))
        {
            Serial.printf("WebInterface: Upload rejected - invalid filename: %s\n", upload.filename.c_str());
            _server.send(400, "text/plain", "Invalid filename");
            return;
        }

        // Validate file extension
        if (!isValidGcodeFile(upload.filename.c_str()))
        {
            Serial.printf("WebInterface: Upload rejected - invalid file type: %s\n", upload.filename.c_str());
            _server.send(400, "text/plain", "Only .gcode files are supported");
            return;
        }

        _currentUploadPath = getPlottingFilePath(upload.filename.c_str());
        _currentTempPath = getTempFilePath(upload.filename.c_str());

        Serial.printf("WebInterface: uploading to - %s\n", _currentUploadPath.c_str());
        Serial.printf("WebInterface: Temp path - %s\n", _currentTempPath.c_str());
        _uploadedBytes = 0;

        // Delete old file if exists
        if (_fileManager.fileExists(_currentUploadPath))
        {
            _fileManager.deleteFile(_currentUploadPath);
        }

        // Delete incomplete temp file if exists
        if (_fileManager.fileExists(_currentTempPath))
        {
            _fileManager.deleteFile(_currentTempPath);
        }

        // Open temp file for writing
        _currentUploadFile = _fileManager.openFileWrite(_currentTempPath);
        if (!_currentUploadFile)
        {
            Serial.printf("WebInterface: ERROR - Failed to create temp file: %s\n", _currentTempPath.c_str());
            _server.send(507, "text/plain", "Insufficient storage");
            return;
        }

        Serial.printf("WebInterface: Upload start - %s (%u bytes total)\n", 
                      upload.filename.c_str(), upload.totalSize);
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        // Check size limit
        if (_uploadedBytes + upload.currentSize > MAX_UPLOAD_SIZE)
        {
            if (_currentUploadFile)
                _currentUploadFile.close();
            if (_fileManager.fileExists(_currentTempPath))
                _fileManager.deleteFile(_currentTempPath);
            
            Serial.printf("WebInterface: Upload rejected - exceeds size limit (%u bytes)\n", MAX_UPLOAD_SIZE);
            _server.send(413, "text/plain", "File too large");
            return;
        }

        // Write chunk to temp file
        if (_currentUploadFile)
        {
            size_t written = _currentUploadFile.write(upload.buf, upload.currentSize);
            if (written != upload.currentSize)
            {
                _currentUploadFile.close();
                _fileManager.deleteFile(_currentTempPath);
                Serial.printf("WebInterface: ERROR - Failed to write upload chunk\n");
                _server.send(500, "text/plain", "Write error");
                return;
            }
            _uploadedBytes += written;
        }
        else
        {
            Serial.println("WebInterface: ERROR - Upload file not open");
            _server.send(500, "text/plain", "Upload not initialized");
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (_currentUploadFile)
        {
            _currentUploadFile.close();
        }

        // Verify upload size matches expected
        if (_uploadedBytes != upload.totalSize)
        {
            _fileManager.deleteFile(_currentTempPath);
            Serial.printf("WebInterface: ERROR - Upload size mismatch (got %u, expected %u)\n", 
                          _uploadedBytes, upload.totalSize);
            _server.send(400, "text/plain", "Upload incomplete or corrupted");
            return;
        }

        // Rename temp file to final destination
        if (!_fileManager.renameFile(_currentTempPath, _currentUploadPath))
        {
            _fileManager.deleteFile(_currentTempPath);
            Serial.printf("WebInterface: ERROR - Failed to finalize upload\n");
            _server.send(500, "text/plain", "Failed to save file");
            return;
        }

        Serial.printf("WebInterface: Upload complete - %s (%u bytes)\n",
                      upload.filename.c_str(), _uploadedBytes);
        
        std::string response = "{\"status\":\"success\",\"file\":\"" + std::string(upload.filename.c_str()) + "\",\"size\":" + std::to_string(_uploadedBytes) + "}";
        _server.send(200, "application/json", response.c_str());

        // Reset upload state
        _currentUploadFile = File();
        _currentUploadPath = "";
        _currentTempPath = "";
        _uploadedBytes = 0;
    }
    else if (upload.status == UPLOAD_FILE_ABORTED)
    {
        if (_currentUploadFile)
        {
            _currentUploadFile.close();
        }

        // Clean up temp file
        if (_fileManager.fileExists(_currentTempPath))
        {
            _fileManager.deleteFile(_currentTempPath);
        }

        Serial.println("WebInterface: Upload aborted by client");
        
        // Reset upload state
        _currentUploadFile = File();
        _currentUploadPath = "";
        _currentTempPath = "";
        _uploadedBytes = 0;
    }
}

bool WebInterface::validateFileName(const std::string& filename)
{
    if (filename.empty() || filename.length() > 255)
        return false;

    // Check for directory traversal attempts
    if (filename.find("..") != std::string::npos || 
        filename.find("/") != std::string::npos ||
        filename.find("\\") != std::string::npos)
        return false;

    // Only alphanumeric, dash, underscore, and dot allowed
    for (char c : filename)
    {
        if (!isalnum(c) && c != '-' && c != '_' && c != '.')
            return false;
    }

    return true;
}

bool WebInterface::isValidGcodeFile(const std::string& filename)
{
    if (filename.length() < 7) // minimum ".gcode"
        return false;

    std::string ext = filename.substr(filename.length() - 6);
    // Convert to lowercase for comparison
    for (char& c : ext) c = tolower(c);

    return ext == ".gcode";
}

std::string WebInterface::getPlottingFilePath(const std::string& filename)
{
    // Construct path from PLOTTING_DIRECTORY + filename
    std::string path = PLOTTING_DIRECTORY + filename;
    
    // Ensure path starts with / for SPIFFS compatibility
    if (path.empty() || path[0] != '/')
    {
        path = "/" + path;
    }
    
    return path;
}

std::string WebInterface::getTempFilePath(const std::string& filename)
{
    // Construct path from TEMP_DIRECTORY + filename
    std::string path = TEMP_DIRECTORY + filename;
    
    // Ensure path starts with / for SPIFFS compatibility
    if (path.empty() || path[0] != '/')
    {
        path = "/" + path;
    }
    
    return path;
}

void WebInterface::handleGetSettings()
{
    char buf[1024];
    std::string json = "{";
    json += "\"mdnsName\":\"" + _runtimeSettings.getMdnsName() + "\"";

    snprintf(buf, sizeof(buf),
        ",\"driverCurrent\":%.2f"
        ",\"microsteps\":%.2f"
        ",\"drawFeedRate\":%.2f"
        ",\"travelFeedRate\":%.2f"
        ",\"homingSpeed\":%.2f"
        ",\"homingBackOffSpeed\":%.2f"
        ",\"stallguardThreshold\":%.2f"
        ",\"backOffStepsX\":%u"
        ",\"backOffStepsY\":%u"
        ",\"homingTimeout\":%u"
        ",\"sgCheckInterval\":%u"
        ",\"sgStartTimeout\":%u"
        ",\"sgHistorySize\":%u"
        ",\"penUpAngle\":%.2f"
        ",\"penDownAngle\":%.2f",
        _runtimeSettings.driverCurrent_mA(),
        _runtimeSettings.microsteps(),
        _runtimeSettings.drawFeedRate_mm_per_s(),
        _runtimeSettings.travelFeedRate_mm_per_s(),
        _runtimeSettings.homingSpeed_stp_per_s(),
        _runtimeSettings.homingBackOffSpeed_stp_per_s(),
        _runtimeSettings.stallguardThreshold(),
        (unsigned int)_runtimeSettings.backOffStepsX(),
        (unsigned int)_runtimeSettings.backOffStepsY(),
        (unsigned int)_runtimeSettings.homingTimeout_us(),
        (unsigned int)_runtimeSettings.sgCheckInterval_ms(),
        (unsigned int)_runtimeSettings.sgStartTimeout_ms(),
        (unsigned int)_runtimeSettings.sgHistorySize(),
        _runtimeSettings.penUpAngle_deg(),
        _runtimeSettings.penDownAngle_deg()
    );

    json += buf;
    json += "}";

    _server.send(200, "application/json", json.c_str());
}

void WebInterface::handleSetSetting()
{
    if (!_server.hasArg("key") || !_server.hasArg("value"))
    {
        _server.send(400, "text/plain", "Missing 'key' or 'value' parameter");
        return;
    }

    const std::string key   = _server.arg("key").c_str();
    const std::string value = _server.arg("value").c_str();

    struct Entry {
        const char* key;
        std::function<void(const std::string&)> setter;
    };

    const Entry table[] = {
        {"mdnsName",            [this](const std::string& v){ _settingPersistence.setMdnsName(v); }},
        {"driverCurrent",       [this](const std::string& v){ _settingPersistence.setDriverCurrent_mA(atof(v.c_str())); }},
        {"microsteps",          [this](const std::string& v){ _settingPersistence.setMicrosteps(atof(v.c_str())); }},
        {"drawFeedRate",        [this](const std::string& v){ _settingPersistence.setDrawFeedRate_mm_per_s(atof(v.c_str())); }},
        {"travelFeedRate",      [this](const std::string& v){ _settingPersistence.setTravelFeedRate_mm_per_s(atof(v.c_str())); }},
        {"homingSpeed",         [this](const std::string& v){ _settingPersistence.setHomingSpeed_stp_per_s(atof(v.c_str())); }},
        {"homingBackOffSpeed",  [this](const std::string& v){ _settingPersistence.setHomingBackOffSpeed_stp_per_s(atof(v.c_str())); }},
        {"stallguardThreshold", [this](const std::string& v){ _settingPersistence.setStallguardThreshold(atof(v.c_str())); }},
        {"backOffStepsX",       [this](const std::string& v){ _settingPersistence.setBackOffStepsX(static_cast<uint16_t>(atoi(v.c_str()))); }},
        {"backOffStepsY",       [this](const std::string& v){ _settingPersistence.setBackOffStepsY(static_cast<uint16_t>(atoi(v.c_str()))); }},
        {"homingTimeout",       [this](const std::string& v){ _settingPersistence.setHomingTimeout_us(static_cast<uint32_t>(atol(v.c_str()))); }},
        {"sgCheckInterval",     [this](const std::string& v){ _settingPersistence.setSGCheckInterval_ms(static_cast<uint16_t>(atoi(v.c_str()))); }},
        {"sgStartTimeout",      [this](const std::string& v){ _settingPersistence.setSGStartTimeout_ms(static_cast<uint16_t>(atoi(v.c_str()))); }},
        {"sgHistorySize",       [this](const std::string& v){ _settingPersistence.setSGHistorySize(static_cast<uint8_t>(atoi(v.c_str()))); }},
        {"penUpAngle",          [this](const std::string& v){ _settingPersistence.setPenUpAngle_deg(atof(v.c_str())); }},
        {"penDownAngle",        [this](const std::string& v){ _settingPersistence.setPenDownAngle_deg(atof(v.c_str())); }},
    };

    for (const auto& entry : table)
    {
        if (key == entry.key)
        {
            entry.setter(value);
            _server.send(200, "text/plain", "OK");
            return;
        }
    }

    _server.send(404, "text/plain", "Unknown setting key");
}