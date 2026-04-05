#pragma once

#include <cstdint>

class Pen {
public:
    virtual void down();
    virtual void up();
    virtual bool isDown() const;

    virtual ~Pen() = default;
};