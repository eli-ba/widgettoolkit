/*
 * Legacy tools for backward compatibility
 */

#pragma once

#include <SDL.h>

class LegacyTools {
public:
    static void Blit(SDL_Surface* source, int x1, int y1, int w, int h, SDL_Surface* dest, int x2, int y2);
    static SDL_Rect Rect(int x, int y, int w, int h);
};
