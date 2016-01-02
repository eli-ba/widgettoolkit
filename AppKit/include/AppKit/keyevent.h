#ifndef _KEYEVENT_H_
#define _KEYEVENT_H_

#include <AppKit/appkit_export.h>
#include <AppKit/inputevent.h>

class APPKIT_API akKeyEvent: public akInputEvent {
public:
	akKeyEvent();
    int GetUnicode();
    int GetKeyEvent();
    void SetUnicode(int unicode);
    void SetKeyEvent(int keyEvent);
private:
    int mUnicode;
    int mKeyEvent;
};



#endif
