#ifndef _MESSAGEBOX_H_
#define _MESSAGEBOX_H_

#include <AppKit/appkit_export.h>
#include <AppKit/window.h>
#include <AppKit/geometry.h>
#include <AppKit/label.h>
#include <AppKit/pushbutton.h>
#include <AppKit/actionreceiver.h>
#include <string>

using namespace std;

class akMessageBoxActionsReceiver : public akActionReceiver
{
public:
	akMessageBoxActionsReceiver(akWindow *wnd);
	virtual void ActionPerformed(akControl *sender);
private:
	akWindow *mWnd;
};

class APPKIT_API akMessageBox
{
public:
	akMessageBox(string title, string message, akWindow *parent = NULL);
private:
	
private:
	string mMessage;
	akLabel *mLabel;
	akPushButton *mButtonOK;
	akWindow *mMsgWnd;
};

#endif
