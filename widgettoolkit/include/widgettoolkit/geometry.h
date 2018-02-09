#pragma once

#include <widgettoolkit/export.h>

class APPKIT_API akPoint {
public:
    akPoint();
    akPoint(int x, int y);

public:
    int x;
    int y;
};

class APPKIT_API akSize {
public:
    akSize();
    akSize(int width, int height);

public:
    int width;
    int height;
};

class APPKIT_API akRect {
public:
    akRect();
    akRect(int x, int y, int width, int height);
    akRect(akPoint location, akSize size);

public:
    akPoint location;
    akSize size;
};

//APPKIT_API akSize akMakeSize(int width, int height);
//APPKIT_API akPoint akMakePoint(int x, int y);
//APPKIT_API akRect akMakeRect(int x, int y, int width, int height);
//APPKIT_API akRect akMakeRect(akPoint location, akSize *size);
