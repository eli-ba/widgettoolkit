#ifndef _MOUSEEVENTRECEIVER_H
#define	_MOUSEEVENTRECEIVER_H

#include <AppKit/appkit_export.h>
#include <AppKit/mouseevent.h>

class akView;

class APPKIT_API akMouseEventReceiver
{
public:
    virtual void MousePress(akView* sender, akMouseEvent *event) = 0;
    virtual void MouseRelease(akView* sender, akMouseEvent *event) = 0;
    virtual void MouseMove(akView* sender, akMouseEvent *event) = 0;
    virtual void MouseDrag(akView* sender, akMouseEvent *event) = 0;
	virtual void MouseWheelUp(akView *sender, akMouseEvent *event) = 0;
	virtual void MouseWheelDown(akView *sender, akMouseEvent *event) = 0;
};

#endif

