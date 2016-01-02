#include <AppKit/pushbutton.h>
#include <AppKit/resources.h>
#include <AppKit/window.h>
#include <AppKit/legacytools.h>
#include <Windows.h>

enum _akButtonState
{
	RELEASED_PUSHBUTTON,
	PRESSED_PUSHBUTTON
};

akPushButton::akPushButton(akRect rect, string text, akView *parent)
	: akControl(rect, parent)
{
	SetClassName("akPushButton");
	mText = text;
	mButtonState = RELEASED_PUSHBUTTON;
	AddPainter(this);
	AddMouseEventReceiver(this);
}

string akPushButton::GetText() 
{
	return mText;
}

void akPushButton::SetText(string text)
{
	mText = text;
	GetWindow()->Repaint();
}

void akPushButton::MousePress(akView* sender, akMouseEvent *event)
{
	GetWindow()->SetFirstResponder(this);
	mButtonState = PRESSED_PUSHBUTTON;
	GetWindow()->Repaint();
}

void akPushButton::MouseRelease(akView* sender, akMouseEvent *event)
{
	InvokeActionReceivers(this);
	mButtonState = RELEASED_PUSHBUTTON;
	GetWindow()->Repaint();
}

void akPushButton::MouseMove(akView* sender, akMouseEvent *event)
{
}

void akPushButton::MouseDrag(akView* sender, akMouseEvent *event)
{
}

void akPushButton::MouseWheelUp(akView *sender, akMouseEvent *event)
{
}

void akPushButton::MouseWheelDown(akView *sender, akMouseEvent *event)
{
}

void akPushButton::Paint(akView *view, SDL_Surface *destination)
{
	SDL_Color black_color = {0,0,0};
	SDL_Surface *surf = NULL,*text=NULL, *nw=NULL, *ne=NULL, 
		*sw=NULL, *se=NULL, *n=NULL, *s=NULL ,*w=NULL, *e=NULL, *img=NULL;
	SDL_Surface *pushbutton_normal = Resources::GetImageResource(PUSHBUTTON_NORMAL);
	SDL_Surface *pushbutton_pressed = Resources::GetImageResource(PUSHBUTTON_PRESSED);
	
	/* Parameters */
	akRect viewRect = GetRect();
	int x = viewRect.location.x;
	int y = viewRect.location.y;
	int width = viewRect.size.width;
	int height = viewRect.size.height;
	TTF_Font *default_app_font = Resources::GetFontResource(DEFAULT_FONT);
	const char *title = mText.c_str();
	
	SDL_Rect p = {x,y}, pt = {x + 5, y + 5}, z={0,0};

	switch(mButtonState)
	{
	case RELEASED_PUSHBUTTON:
		img = pushbutton_normal;
		break;

	case PRESSED_PUSHBUTTON:
		img = pushbutton_pressed;
		break;

	default:
		return;
	}

	text = TTF_RenderText_Blended(default_app_font, title, black_color);
	surf = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_HWACCEL, width, height, 32, 0, 0, 0, 0);
	SDL_FillRect(surf, NULL, SDL_MapRGB(SDL_GetVideoSurface()->format, 0, 255, 0));

	//Fill the whole surface
	for (int i=0; i<surf->w; i++)
	{
		for (int j=0; j<surf->h; j++)
		{
			LegacyTools::Blit(img,3+1,3+1,1,1,surf,i,j);
		}
	}

	LegacyTools::Blit(img,0,0,3,3,surf,0,0);
	LegacyTools::Blit(img,img->w - 3,0,3,3,surf,surf->w - 3,0);
	LegacyTools::Blit(img,0,img->h - 3,3,3,surf,0,surf->h - 3);
	LegacyTools::Blit(img,img->w - 3,img->h - 3,3,3,surf,surf->w - 3, surf->h - 3);

	for (int i=0; i<surf->w - 3*2; i++)
	{
		LegacyTools::Blit(img,3,0,1,3,surf,i+3,0);
		LegacyTools::Blit(img,3,img->h - 3,1,3,surf,i+3,surf->h - 3);
	}

	for (int i=0; i<surf->h - 3*2; i++)
	{
		LegacyTools::Blit(img,0,3,3,1,surf,0,i+3);
		LegacyTools::Blit(img,img->w - 3,3,3,1,surf,surf->w - 3,i+3);
	}

	//MDDrawSurface(surf,dest_wnd,p.x,p.y,0);
	SDL_Rect dstrect;
	dstrect.x = p.x;
	dstrect.y = p.y;
	SDL_BlitSurface(surf, NULL, destination, &dstrect);

	pt.x = x + (width / 2) - (text->w / 2);
	pt.y = y + (height / 2) - (text->h / 2);

	//Text pressed effect
	if(mButtonState == PRESSED_PUSHBUTTON)
	{
		pt.x += 1;
		pt.y += 1;
	}

	//MDDrawSurface(text,dest_wnd,pt.x,pt.y,1);
	dstrect;
	dstrect.x = pt.x;
	dstrect.y = pt.y;
	SDL_BlitSurface(text, NULL, destination, &dstrect);

	SDL_FreeSurface(text);
	SDL_FreeSurface(surf);
}

