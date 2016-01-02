#ifndef _KEYEVENTRECEIVER_H
#define	_KEYEVENTRECEIVER_H

#include <AppKit/appkit_export.h>
#include <AppKit/keyevent.h>

class akView;

class APPKIT_API akKeyEventReceiver {
public:
    virtual void KeyPress(akView* sender, akKeyEvent *event) = 0;
	virtual void KeyRelease(akView* sender, akKeyEvent *event) = 0;
};

#endif

