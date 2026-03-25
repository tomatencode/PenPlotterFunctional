#pragma once
#include <optional>


template<typename T>
class FreeRtosQueue {
public:
    FreeRtosQueue(size_t capacity)
        : _capacity(capacity),
        _queue(xQueueCreate(capacity, sizeof(T)))
    {}

    ~FreeRtosQueue() {
        if (_queue) {
            vQueueDelete(_queue);
        }
    }

    [[nodiscard]] bool trySend(const T& item, TickType_t timeout = portMAX_DELAY) {
        if (!_queue) return false;
        return xQueueSend(_queue, &item, timeout) == pdPASS;
    }

    [[nodiscard]] std::optional<T> tryReceive(TickType_t timeout = portMAX_DELAY) {
        if (!_queue) return std::nullopt;
        T item;
        if (xQueueReceive(_queue, &item, timeout) == pdPASS) {
            return item;
        }
        return std::nullopt;
    }

    void clear() {
        if (!_queue) return;
        xQueueReset(_queue);
    }

    size_t messagesWaiting() const {
        if (!_queue) return 0;
        return uxQueueMessagesWaiting(_queue);
    }

    size_t spacesAvailable() const {
        if (!_queue) return 0;
        return uxQueueSpacesAvailable(_queue);
    }

    size_t capacity() const {
        if (!_queue) return 0;
        return _capacity;
    }

    FreeRtosQueue(const FreeRtosQueue&) = delete;
    FreeRtosQueue& operator=(const FreeRtosQueue&) = delete;

    FreeRtosQueue(FreeRtosQueue&& other) noexcept
        : _queue(other._queue),
          _capacity(other._capacity)
    {
        other._queue = nullptr;
        other._capacity = 0;
    }

    FreeRtosQueue& operator=(FreeRtosQueue&& other) noexcept
    {
        if (this != &other) {
            if (_queue) vQueueDelete(_queue);
            _queue = other._queue;
            _capacity = other._capacity;
            other._queue = nullptr;
            other._capacity = 0;
        }
        return *this;
    }

private:
    QueueHandle_t _queue;
    size_t _capacity;
};