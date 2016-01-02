#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <AppKit/geometry.h>

class Utility
{
public:
	static bool BitTest(long value, int bitIndex);
	static bool RectContainsPoint(akRect rect, akPoint point);
};

#endif