#pragma once

#include "../framework/screen/Screen.hpp"
#include "../components/HeaderLine.hpp"

// A simple secondary screen used to demonstrate navigation.
class SecondaryScreen : public Screen
{
public:
    SecondaryScreen();

private:
    HeaderLine* header;
};
