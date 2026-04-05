#pragma once

#include <cstdint>

class Pen {
public:
    virtual void down() = 0;
    virtual void up() = 0;
    virtual bool isDown() const = 0;

    virtual ~Pen() = default;
};