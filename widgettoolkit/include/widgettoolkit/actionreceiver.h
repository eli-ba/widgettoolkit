#pragma once

#include <widgettoolkit/export.h>

class akControl;

class APPKIT_API akActionReceiver {
public:
    virtual void ActionPerformed(akControl* sender) = 0;
};
