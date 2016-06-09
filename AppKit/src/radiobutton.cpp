#include <AppKit/radiobutton.h>
#include <AppKit/resources.h>
#include <AppKit/window.h>
#include <AppKit/aktypes.h>
#include <SDL_ttf.h>

enum _akRadioButtonStates
{
	STATE1_RADIOBUTTON,
	STATE2_RADIOBUTTON,
	STATE3_RADIOBUTTON
};

akRadioButtonGroup::akRadioButtonGroup()
{
}

void akRadioButtonGroup::Add(akRadioButton *radioButton)
{
	radioButton->SetGroup(this);
	mButtons.push_back(radioButton);
}

void akRadioButtonGroup::Remove(akRadioButton *radioButton)
{
	radioButton->SetGroup(NULL);
	for (unsigned int i=0; i<mButtons.size(); i++) {
		akRadioButton *radioToRemove = mButtons.at(i);
		if (radioToRemove == radioButton) {
			mButtons.erase(mButtons.begin()+i);
			return;
		}
	}
}

vector<akRadioButton*> akRadioButtonGroup::GetButtons()
{
	return mButtons;
}

akRadioButton::akRadioButton(akRect rect, string text, akView *parent)
	: akControl(rect, parent)
{
	SetClassName("akRadioButton");
	mButtonState = STATE1_RADIOBUTTON;
	mText = text;
	mGroup = NULL;

	AddMouseEventReceiver(this);
	AddKeyEventReceiver(this);
	AddPainter(this);
}

void akRadioButton::SetGroup(akRadioButtonGroup *group)
{
	mGroup = group;
}

void akRadioButton::SetText(string text)
{
	mText = text;
}

string akRadioButton::GetText()
{
	return mText;
}

void akRadioButton::SetSelected(bool selected)
{
	if (selected) {
		mButtonState = STATE3_RADIOBUTTON;
	}
	else {
		mButtonState = STATE1_RADIOBUTTON;
	}
	GetWindow()->Repaint();
}

bool akRadioButton::IsSelected()
{
	if (mButtonState == STATE3_RADIOBUTTON) return true;
	else return false;
}

/* akMouseEventReceiver overrides */
void akRadioButton::MousePress(akView* sender, akMouseEvent *event)
{
	if (mButtonState == STATE1_RADIOBUTTON)
	{
		mButtonState = STATE3_RADIOBUTTON;
		GetWindow()->SetFirstResponder(this);
		this->InvokeActionReceivers(this);

		if (mGroup) {
			vector<akRadioButton*> buttons = mGroup->GetButtons();
			for (unsigned int i=0; i<buttons.size(); i++) {
				akRadioButton *radio = buttons.at(i);
				if (radio != this) {
					radio->mButtonState = STATE1_RADIOBUTTON;
					radio->InvokeActionReceivers(radio);
				}
			}
		}
		GetWindow()->Repaint();
	}
}

void akRadioButton::MouseRelease(akView* sender, akMouseEvent *event)
{
}

void akRadioButton::MouseMove(akView* sender, akMouseEvent *event)
{
}

void akRadioButton::MouseDrag(akView* sender, akMouseEvent *event)
{
}

void akRadioButton::MouseWheelUp(akView *sender, akMouseEvent *event)
{
}

void akRadioButton::MouseWheelDown(akView *sender, akMouseEvent *event)
{
}

/* akKeyEventReceiver overrides */
void akRadioButton::KeyPress(akView* sender, akKeyEvent *event)
{
}

void akRadioButton::KeyRelease(akView* sender, akKeyEvent *event)
{
}

void akRadioButton::TextInput(akView* sender, akKeyEvent *event)
{
}

/* akPainter overrides */
void akRadioButton::Paint(akView *view, SDL_Surface *destination)
{
	SDL_Color black_color = {0,0,0};
	SDL_Surface *surf = NULL,*text=NULL,*img=NULL;
	akRect viewRect = GetRect();
	int x = viewRect.location.x;
	int y = viewRect.location.y;
	int width = viewRect.size.width;
	int height = viewRect.size.height;
	SDL_Surface *radiobutton_state1 = Resources::GetImageResource(RADIOBUTTON_STATE1);
	SDL_Surface *radiobutton_state2 = Resources::GetImageResource(RADIOBUTTON_STATE2);
	SDL_Surface *radiobutton_state3 = Resources::GetImageResource(RADIOBUTTON_STATE3);

	SDL_Rect p = {x,y}, pt = {x + 5, y + 5}, z={0,0};

	switch(mButtonState)
	{
		case STATE1_RADIOBUTTON:
			img = radiobutton_state1;
			break;

		case STATE2_RADIOBUTTON:
			img = radiobutton_state2;
			break;

		case STATE3_RADIOBUTTON:
			img = radiobutton_state3;
			break;

		default:
			break;
	}

	text = TTF_RenderText_Blended(Resources::GetFontResource(DEFAULT_FONT), mText.c_str(), black_color);
	//SDL_SetColorKey(img, SDL_SRCCOLORKEY, SDL_MapRGB(img->format, 255, 0, 255));
	surf = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	akColor defaultBgColor = GetWindow()->GetBackgroundColor();
	SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format,defaultBgColor.r,defaultBgColor.g,defaultBgColor.b));
	z.y += (height / 2) - (img->h /2);
	z.x += 2;
	SDL_BlitSurface(img,NULL,surf,&z);
	pt.x = img->w + 9;
	pt.y = 3;
	SDL_BlitSurface(text,NULL,surf,&pt);
	
	SDL_Rect dstrect;
	dstrect.x = p.x;
	dstrect.y = p.y;
	SDL_BlitSurface(surf, NULL, destination, &dstrect);

	SDL_FreeSurface(surf);
	SDL_FreeSurface(text);
}