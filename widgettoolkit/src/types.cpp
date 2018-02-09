#include <widgettoolkit/types.h>

akColor::akColor()
{
    r = 0;
    g = 0;
    b = 0;
}

akColor::akColor(int r, int g, int b, int a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}
