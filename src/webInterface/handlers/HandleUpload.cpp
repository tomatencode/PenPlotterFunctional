#include "../WebInterface.hpp"
#include "config/directorys_config.hpp"

// ── Private helpers ──────────────────────────────────────────────────────────

bool WebInterface::validateFileName(const std::string& filename)
{
    if (filename.empty() || filename.length() > 255)
        return false;

    if (filename.find("..") != std::string::npos ||
        filename.find("/")  != std::string::npos ||
        filename.find("\\") != std::string::npos)
        return false;

    for (char c : filename)
    {
        if (!isalnum(c) && c != '-' && c != '_' && c != '.')
            return false;
    }

    return true;
}

bool WebInterface::isValidGcodeFile(const std::string& filename)
{
    if (filename.length() < 7) // minimum: "x.gcode"
        return false;

    std::string ext = filename.substr(filename.length() - 6);
    for (char& c : ext) c = tolower(c);

    return ext == ".gcode";
}

std::string WebInterface::getPlottingFilePath(const std::string& filename)
{
    return PLOTTING_DIRECTORY + filename;
}

std::string WebInterface::getTempFilePath(const std::string& filename)
{
    return TEMP_DIRECTORY + filename;
}

void WebInterface::resetUploadState()
{
    _currentUploadFile = File();
    _currentUploadPath = "";
    _currentTempPath   = "";
    _uploadedBytes     = 0;
}

// ── Handler ──────────────────────────────────────────────────────────────────

void WebInterface::handleUpload()
{
    HTTPUpload& upload = _server.upload();

    if (upload.status == UPLOAD_FILE_START)
    {
        if (!validateFileName(upload.filename.c_str()))
        {
            Serial.printf("WebInterface: Upload rejected - invalid filename: %s\n", upload.filename.c_str());
            _server.send(400, "text/plain", "Invalid filename");
            return;
        }

        if (!isValidGcodeFile(upload.filename.c_str()))
        {
            Serial.printf("WebInterface: Upload rejected - invalid file type: %s\n", upload.filename.c_str());
            _server.send(400, "text/plain", "Only .gcode files are supported");
            return;
        }

        _currentUploadPath = getPlottingFilePath(upload.filename.c_str());
        _currentTempPath   = getTempFilePath(upload.filename.c_str());
        _uploadedBytes     = 0;

        if (_fileManager.fileExists(_currentUploadPath))
            _fileManager.deleteFile(_currentUploadPath);

        if (_fileManager.fileExists(_currentTempPath))
            _fileManager.deleteFile(_currentTempPath);

        _currentUploadFile = _fileManager.openFileWrite(_currentTempPath);
        if (!_currentUploadFile)
        {
            Serial.printf("WebInterface: Failed to create temp file: %s\n", _currentTempPath.c_str());
            _server.send(507, "text/plain", "Insufficient storage");
            return;
        }

        Serial.printf("WebInterface: Upload start - %s (%u bytes)\n", upload.filename.c_str(), upload.totalSize);
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        if (_uploadedBytes + upload.currentSize > MAX_UPLOAD_SIZE)
        {
            if (_currentUploadFile) _currentUploadFile.close();
            if (_fileManager.fileExists(_currentTempPath)) _fileManager.deleteFile(_currentTempPath);

            Serial.printf("WebInterface: Upload rejected - exceeds %u byte limit\n", MAX_UPLOAD_SIZE);
            _server.send(413, "text/plain", "File too large");
            return;
        }

        if (!_currentUploadFile)
        {
            Serial.println("WebInterface: Upload file not open");
            _server.send(500, "text/plain", "Upload not initialized");
            return;
        }

        size_t written = _currentUploadFile.write(upload.buf, upload.currentSize);
        if (written != upload.currentSize)
        {
            _currentUploadFile.close();
            _fileManager.deleteFile(_currentTempPath);
            Serial.println("WebInterface: Failed to write upload chunk");
            _server.send(500, "text/plain", "Write error");
            return;
        }

        _uploadedBytes += written;
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (_currentUploadFile) _currentUploadFile.close();

        if (_uploadedBytes != upload.totalSize)
        {
            _fileManager.deleteFile(_currentTempPath);
            Serial.printf("WebInterface: Upload size mismatch (got %u, expected %u)\n",
                          _uploadedBytes, upload.totalSize);
            _server.send(400, "text/plain", "Upload incomplete or corrupted");
            resetUploadState();
            return;
        }

        if (!_fileManager.renameFile(_currentTempPath, _currentUploadPath))
        {
            _fileManager.deleteFile(_currentTempPath);
            Serial.println("WebInterface: Failed to finalize upload");
            _server.send(500, "text/plain", "Failed to save file");
            resetUploadState();
            return;
        }

        Serial.printf("WebInterface: Upload complete - %s (%u bytes)\n",
                      upload.filename.c_str(), _uploadedBytes);

        std::string response = "{\"status\":\"success\",\"file\":\""
                             + std::string(upload.filename.c_str())
                             + "\",\"size\":" + std::to_string(_uploadedBytes) + "}";
        _server.send(200, "application/json", response.c_str());

        resetUploadState();
    }
    else if (upload.status == UPLOAD_FILE_ABORTED)
    {
        if (_currentUploadFile) _currentUploadFile.close();
        if (_fileManager.fileExists(_currentTempPath)) _fileManager.deleteFile(_currentTempPath);

        Serial.println("WebInterface: Upload aborted by client");
        resetUploadState();
    }
}
