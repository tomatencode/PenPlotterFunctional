#ifndef PEN_HPP
#define PEN_HPP

#include <cstdint>

class Pen {
public:
    virtual void down();
    virtual void up();

    virtual ~Pen() = default;
};

#endif