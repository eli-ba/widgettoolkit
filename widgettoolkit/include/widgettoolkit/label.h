#pragma once

#include <widgettoolkit/export.h>
#include <widgettoolkit/view.h>
#include <widgettoolkit/geometry.h>
#include <widgettoolkit/painter.h>
#include <widgettoolkit/control.h>
#include <string>

class APPKIT_API akLabel : public akControl, akPainter {
public:
    akLabel(akRect rect, string text, akView* parent = NULL);
    void SetText(string text);
    string GetText();

private:
    virtual void Paint(akView* view, SDL_Surface* destination);

private:
    string mText;
    SDL_Surface* mTextSurface;
};
