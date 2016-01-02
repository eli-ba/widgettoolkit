#ifndef _ACTIONRECEIVER_H_
#define _ACTIONRECEIVER_H_

#include <AppKit/appkit_export.h>

class akControl;

class APPKIT_API akActionReceiver
{
public:
	virtual void ActionPerformed(akControl *sender) = 0;
};

#endif
