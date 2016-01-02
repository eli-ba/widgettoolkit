#ifndef _INPUTEVENT_H_
#define _INPUTEVENT_H_

#include <AppKit/appkit_export.h>
#include <AppKit/constants.h>
#include <AppKit/object.h>

class akWindow;

class APPKIT_API akInputEvent : public akObject
{
public:
	akInputEvent();
	void SetWindow(akWindow *wnd);
	akWindow *GetWindow();
	void SetType(int type);
	int GetType();
private:
	akWindow *mWindow;
	int mType;
public:
	void *Reserved;
};

#endif 
