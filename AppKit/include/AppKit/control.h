#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <AppKit/appkit_export.h>
#include <AppKit/view.h>
#include <AppKit/actionreceiver.h>
#include <vector>

class APPKIT_API akControl : public akView
{
public:
	akControl(akRect rect, akView *parent = NULL);
	void AddActionReceiver(akActionReceiver *receiver);
	void RemoveActionReceiver(akActionReceiver *receiver);
	void InvokeActionReceivers(akControl *sender);
private:
	vector<akActionReceiver*> mActionReceivers;
};

#endif
