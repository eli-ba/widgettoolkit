#include <widgettoolkit/label.h>
#include <widgettoolkit/window.h>
#include <widgettoolkit/resources.h>
#include <SDL_ttf.h>

akLabel::akLabel(akRect rect, string text, akView* parent /*= NULL*/)
    : akControl(rect, parent)
{
    mText = text;
    AddPainter(this);
}

void akLabel::SetText(string text)
{
    mText = text;
    GetWindow()->Repaint();
}

string akLabel::GetText()
{
    return mText;
}

void akLabel::Paint(akView* view, SDL_Surface* destination)
{
    SDL_Color blackColor;
    blackColor.r = blackColor.g = blackColor.b = 0;
    mTextSurface = TTF_RenderText_Blended(Resources::GetFontResource(DEFAULT_FONT), mText.c_str(), blackColor);

    akRect viewRect = GetRect();
    SDL_Rect dstrect;
    dstrect.x = viewRect.location.x;
    dstrect.y = viewRect.location.y;
    dstrect.w = mTextSurface->w;
    dstrect.h = mTextSurface->h;
    SDL_BlitSurface(mTextSurface, NULL, destination, &dstrect);

    SDL_FreeSurface(mTextSurface);
}
