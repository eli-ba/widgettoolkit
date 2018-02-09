#include <widgettoolkit/keyevent.h>
#include <widgettoolkit/constants.h>

akKeyEvent::akKeyEvent()
{
    SetClassName("akKeyEvent");
    SetType(akINPUT_EVENT_KEY);
}
/*
int akKeyEvent::GetUnicode()
{
	return mUnicode;
}
*/
int akKeyEvent::GetKeyEvent()
{
    return mKeyEvent;
}
/*
void akKeyEvent::SetUnicode(int unicode)
{
	mUnicode = unicode;
}
*/
void akKeyEvent::SetKeyEvent(int keyEvent)
{
    mKeyEvent = keyEvent;
}

void akKeyEvent::SetText(const std::string text)
{
    mText = text;
}

std::string const& akKeyEvent::GetText()
{
    return mText;
}
