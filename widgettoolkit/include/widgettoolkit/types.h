#pragma once

#include <widgettoolkit/export.h>

class APPKIT_API akColor {
public:
    akColor();
    akColor(int r, int g, int b, int a);

public:
    int r;
    int g;
    int b;
    int a;
};
