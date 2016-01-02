#include <stdlib.h>
#include <AppKit/inputevent.h>

akInputEvent::akInputEvent()
{
	SetClassName("akInputEvent");
	mWindow = NULL;
}

void akInputEvent::SetWindow(akWindow *wnd)
{
	mWindow = wnd;
}

void akInputEvent::SetType(int type)
{
	mType = type;
}

akWindow* akInputEvent::GetWindow()
{
	return mWindow;
}

int akInputEvent::GetType()
{
	return mType;
}