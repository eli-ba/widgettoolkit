#ifndef _LABEL_H
#define _LABEL_H

#include <AppKit/appkit_export.h>
#include <AppKit/view.h>
#include <AppKit/geometry.h>
#include <AppKit/painter.h>
#include <AppKit/control.h>
#include <string>

class APPKIT_API akLabel : public akControl, akPainter
{
public:
	akLabel(akRect rect, string text, akView *parent = NULL);
	void SetText(string text);
	string GetText();
private:
	virtual void Paint(akView *view, SDL_Surface *destination);
private:
	string mText;
	SDL_Surface *mTextSurface;
};

#endif
