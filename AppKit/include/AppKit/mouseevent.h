#ifndef _MOUSEEVENT_H_
#define _MOUSEEVENT_H_

#include <AppKit/appkit_export.h>
#include <AppKit/constants.h>
#include <AppKit/inputevent.h>
#include <AppKit/geometry.h>

class APPKIT_API akMouseEvent : public akInputEvent
{
public:
    akMouseEvent();
    int GetMouseEvent();
	int GetButton();
    akPoint GetLocation();
	akPoint GetLocationInWindow();
	void SetMouseEvent(int mouseEvent);
	void SetButton(int button);
	void SetLocation(akPoint location);
	void SetLocationInWindow(akPoint location);

private:
	int mMouseEvent;
	int mButton;
	akPoint mLocation;
	akPoint mLocationInWindow;
};


#endif
