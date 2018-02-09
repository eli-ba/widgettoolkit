#include <widgettoolkit/utility.h>

bool Utility::BitTest(long n, int bitIndex)
{
    int d = 1;

    d = d << bitIndex;
    if ((n & d) == d)
        return true;
    else
        return false;
}

bool Utility::RectContainsPoint(akRect rect, akPoint point)
{
    if (point.x >= rect.location.x && point.y >= rect.location.y && point.x <= rect.location.x + rect.size.width && point.y <= rect.location.y + rect.size.height) {
        return true;
    }
    else
        return false;
}
