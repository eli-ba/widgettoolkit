#pragma once

#include <widgettoolkit/export.h>
#include <SDL.h>

class akView;

class APPKIT_API akPainter {
public:
    virtual void Paint(akView* view, SDL_Surface* destination) = 0;
};
