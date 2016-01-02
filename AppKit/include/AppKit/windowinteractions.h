#ifndef _WINDOWINTERACTIONS_H_
#define _WINDOWINTERACTIONS_H_

#include <AppKit/inputevent.h>
#include <AppKit/basewindow.h>

class WindowManager;

class WindowInteractions
{
public:
	WindowInteractions(WindowManager *wm);
	void OnInputEvent(akInputEvent *evt);
	BaseWindow* GetTopLevelWindow();
	BaseWindow* GetWindowThatContainsPoint(akPoint point);
public:
	WindowManager *mWindowManager;
	BaseWindow *mTopLevelWindow;
	BaseWindow *mCapturedWindow;
};


#endif
