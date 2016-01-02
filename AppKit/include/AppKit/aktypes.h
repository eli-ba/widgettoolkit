#ifndef _AKTYPES_H
#define _AKTYPES_H

#include <AppKit/appkit_export.h>

class APPKIT_API akColor
{
public:
	akColor();
	akColor(int r, int g, int b, int a);
public:
	int r;
	int g;
	int b;
	int a;
};

#endif
