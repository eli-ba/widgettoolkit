#ifndef _PAINTER_H_
#define _PAINTER_H_

#include <AppKit/appkit_export.h>
#include <SDL.h>

class akView;

class APPKIT_API akPainter
{
public:
	virtual void Paint(akView *view, SDL_Surface *destination) = 0;
};

#endif
