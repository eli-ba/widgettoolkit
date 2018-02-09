#pragma once

#include <widgettoolkit/geometry.h>

class Utility {
public:
    static bool BitTest(long value, int bitIndex);
    static bool RectContainsPoint(akRect rect, akPoint point);
};
