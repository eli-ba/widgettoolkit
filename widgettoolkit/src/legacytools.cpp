#include <widgettoolkit/legacytools.h>

void LegacyTools::Blit(SDL_Surface* source, int x1, int y1, int w, int h, SDL_Surface* dest, int x2, int y2)
{
    SDL_Rect s = Rect(x1, y1, w, h);
    SDL_Rect d = Rect(x2, y2, w, h);
    SDL_Rect* ps = &s;
    if (x1 < 0)
        ps = NULL;
    SDL_BlitSurface(source, ps, dest, &d);
}

SDL_Rect LegacyTools::Rect(int x, int y, int w, int h)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    return r;
}
