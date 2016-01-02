#include <AppKit/keyevent.h>
#include <AppKit/constants.h>

akKeyEvent::akKeyEvent()
{
	SetClassName("akKeyEvent");
	SetType(akINPUT_EVENT_KEY);
}

int akKeyEvent::GetUnicode()
{
	return mUnicode;
}

int akKeyEvent::GetKeyEvent()
{
	return mKeyEvent;
}

void akKeyEvent::SetUnicode(int unicode)
{
	mUnicode = unicode;
}

void akKeyEvent::SetKeyEvent(int keyEvent)
{
	mKeyEvent = keyEvent;
}