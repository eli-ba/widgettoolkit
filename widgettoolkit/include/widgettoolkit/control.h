#pragma once

#include <widgettoolkit/export.h>
#include <widgettoolkit/view.h>
#include <widgettoolkit/actionreceiver.h>
#include <vector>

class APPKIT_API akControl : public akView {
public:
    akControl(akRect rect, akView* parent = NULL);
    void AddActionReceiver(akActionReceiver* receiver);
    void RemoveActionReceiver(akActionReceiver* receiver);
    void InvokeActionReceivers(akControl* sender);

private:
    vector<akActionReceiver*> mActionReceivers;
};
