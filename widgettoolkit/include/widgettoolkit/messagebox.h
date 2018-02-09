#pragma once

#include <widgettoolkit/export.h>
#include <widgettoolkit/window.h>
#include <widgettoolkit/geometry.h>
#include <widgettoolkit/label.h>
#include <widgettoolkit/pushbutton.h>
#include <widgettoolkit/actionreceiver.h>
#include <string>

using namespace std;

class akMessageBoxActionsReceiver : public akActionReceiver {
public:
    akMessageBoxActionsReceiver(akWindow* wnd);
    virtual void ActionPerformed(akControl* sender);

private:
    akWindow* mWnd;
};

class APPKIT_API akMessageBox {
public:
    akMessageBox(string title, string message, akWindow* parent = NULL);

private:
private:
    string mMessage;
    akLabel* mLabel;
    akPushButton* mButtonOK;
    akWindow* mMsgWnd;
};
