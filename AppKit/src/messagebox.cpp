#include <AppKit/messagebox.h>
#include <AppKit/windowmanager.h>

extern WindowManager *TheWindowManager;

#define akMESSAGEBOX_WIDTH 250
#define akMESSAGEBOX_HEIGHT 80

akMessageBoxActionsReceiver::akMessageBoxActionsReceiver(akWindow *wnd)
{
	this->mWnd = wnd;
}

void akMessageBoxActionsReceiver::ActionPerformed(akControl *sender)
{
	mWnd->Close();
}

akMessageBox::akMessageBox(string title, string message, akWindow *parent)
{
	mMsgWnd = new akWindow(akRect(TheWindowManager->GetResolution().width/2 - akMESSAGEBOX_WIDTH/2,
								  TheWindowManager->GetResolution().height/2 - akMESSAGEBOX_HEIGHT/2,
								  akMESSAGEBOX_WIDTH,
								  akMESSAGEBOX_HEIGHT), 
							title, akWS_CLOSABLE);

	mMessage = message;
	mLabel = new akLabel(akRect(10,10, akMESSAGEBOX_WIDTH - 20, 25), message);

#define akMB_PUSHBUTTON_WIDTH 120
#define akMB_PUSHBUTTON_HEIGHT 30

	mButtonOK = new akPushButton(akRect(akMESSAGEBOX_WIDTH/2 - akMB_PUSHBUTTON_WIDTH/2,
										akMESSAGEBOX_HEIGHT/2 - akMB_PUSHBUTTON_HEIGHT/2 + 15,
										akMB_PUSHBUTTON_WIDTH,
										akMB_PUSHBUTTON_HEIGHT),
								"OK");
	mButtonOK->AddActionReceiver(new akMessageBoxActionsReceiver(mMsgWnd));
	
	mMsgWnd->AddView(mLabel);
	mMsgWnd->AddView(mButtonOK);
	mMsgWnd->SetVisible(true);
}

