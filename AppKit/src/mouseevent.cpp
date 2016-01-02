#include <AppKit/mouseevent.h>

akMouseEvent::akMouseEvent()
{
	SetClassName("akMouseEvent");
	SetType(akINPUT_EVENT_MOUSE);
}

int akMouseEvent::GetMouseEvent()
{
	return mMouseEvent;
}

int akMouseEvent::GetButton()
{
	return mButton;
}

akPoint akMouseEvent::GetLocation()
{
	return mLocation;
}

akPoint akMouseEvent::GetLocationInWindow()
{
	return mLocationInWindow;
}

void akMouseEvent::SetMouseEvent(int mouseEvent)
{
	mMouseEvent = mouseEvent;
}

void akMouseEvent::SetButton(int button)
{
	mButton = button;
}

void akMouseEvent::SetLocation(akPoint location)
{
	mLocation = location;
}

void akMouseEvent::SetLocationInWindow(akPoint location)
{
	mLocationInWindow = location;
}

