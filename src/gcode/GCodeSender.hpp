#pragma once

#include <optional>
#include <cstring>
#include <string>

#include <esp_log.h>

#include "rtos/RtosQueue.hpp"
#include "rtos/GcodeMessage.hpp"

class GCodeSender {
public:
    GCodeSender(RtosQueue<GcodeMessage>& gcodeQueue)
        : _gcodeQueue(gcodeQueue)
    {}

    class Token {
    public:
        Token(const Token&) = delete;
        Token& operator=(const Token&) = delete;

        Token(Token&& other) noexcept : _sender(other._sender) {
            other._sender = nullptr;
        }

        Token& operator=(Token&& other) noexcept {
            if (this != &other) {
                if (_sender) _sender->_doRelease(); // release current before taking ownership
                _sender = other._sender;
                other._sender = nullptr;
            }
            return *this;
        }

        ~Token() {
            if (_sender) _sender->_doRelease();
        }

        bool send(const std::string& line) {
            if (!_sender) ESP_LOGE("GCodeSender", "trying to send line with invalid token");
            return _sender->_doSend(line);
        }

        size_t messagesWaiting() const {
            if (!_sender) ESP_LOGE("GCodeSender", "trying to get messagesWaiting with invalid token");
            return _sender->_doMessagesWaiting();
        }

        size_t spacesAvailable() const {
            if (!_sender) ESP_LOGE("GCodeSender", "trying to get spacesAvailable with invalid token");
            return _sender->_doSpacesAvailable();
        }

        void clearQueue() {
            if (!_sender) ESP_LOGE("GCodeSender", "trying to clearQueue with invalid token");
            _sender->_doClearQueue();
        }

    private:
        GCodeSender* _sender;
        Token(GCodeSender* sender) : _sender(sender) {}
        friend class GCodeSender;
    };

    std::optional<Token> tryAcquire() {
        ESP_LOGD("GCodeSender", "Attempting to acquire token");
        if (_locked) return std::nullopt;
        _locked = true;
        ESP_LOGD("GCodeSender", "Token acquired");
        return Token{this};
    }

private:
    bool _locked = false;

    RtosQueue<GcodeMessage>& _gcodeQueue;


    void _doRelease() {
        ESP_LOGD("GCodeSender", "Token released");
        _locked = false;
    }

    bool _doSend(const std::string& line) {
        GcodeMessage msg;
        strncpy(msg.line, line.c_str(), MAX_GCODE_LINE - 1);
        msg.line[MAX_GCODE_LINE - 1] = '\0';
        return _gcodeQueue.trySend(msg, 0);
    }

    size_t _doMessagesWaiting() const {
        return _gcodeQueue.messagesWaiting();
    }

    size_t _doSpacesAvailable() const {
        return _gcodeQueue.spacesAvailable();
    }

    void _doClearQueue() {
        _gcodeQueue.clear();
    }
};