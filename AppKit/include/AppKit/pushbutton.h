#ifndef _PUSHBUTTON_H_
#define _PUSHBUTTON_H_

#include <AppKit/appkit_export.h>
#include <AppKit/view.h>
#include <AppKit/geometry.h>
#include <AppKit/mouseevent.h>
#include <AppKit/mouseeventreceiver.h>
#include <AppKit/actionreceiver.h>
#include <AppKit/painter.h>
#include <AppKit/control.h>
#include <string>

using namespace std;

class APPKIT_API akPushButton : public akControl, akMouseEventReceiver, akPainter
{
public:
	akPushButton(akRect rect, string text, akView *parent = NULL);
	string GetText();
	void SetText(string text);
private:
	virtual void MousePress(akView* sender, akMouseEvent *event);
    virtual void MouseRelease(akView* sender, akMouseEvent *event);
    virtual void MouseMove(akView* sender, akMouseEvent *event);
    virtual void MouseDrag(akView* sender, akMouseEvent *event);
	virtual void MouseWheelUp(akView *sender, akMouseEvent *event);
	virtual void MouseWheelDown(akView *sender, akMouseEvent *event);
	virtual void Paint(akView *view, SDL_Surface *destination);
private:
	string mText;
	int mButtonState;
};

#endif