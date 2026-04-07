#include "WebInterface.hpp"
#include "config/job_config.hpp"

#include <string>
#include <functional>
#include <cstdlib>

void WebInterface::handleFileList()
{
    auto files = _fileManager.listFiles(PLOTTING_DIRECTORY + "/");

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
    std::string filepath = PLOTTING_DIRECTORY + "/" + std::string(filename.c_str());

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
        std::string path = std::string("/") + upload.filename.c_str();
        Serial.printf("WebInterface: Upload start - %s\n", path.c_str());

        if (_fileManager.fileExists(path))
            _fileManager.deleteFile(path);

        File f = _fileManager.openFileWrite(path);
        if (!f)
            Serial.println("WebInterface: ERROR - Failed to open file for writing");

        f.close();
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        std::string path = std::string("/") + upload.filename.c_str();
        File f = _fileManager.openFileWrite(path);
        f.write(upload.buf, upload.currentSize);
        f.close();
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        Serial.printf("WebInterface: Upload complete - %s (%u bytes)\n",
                      upload.filename.c_str(), upload.totalSize);
        _server.send(200, "text/plain", "File uploaded successfully");
    }
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
